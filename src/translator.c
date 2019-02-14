/*
 * translator.c
 *
 *  Created on: 9 ����. 2019 �.
 *      Author: Exucutional
 */

#include "asprintf.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "tree_t.h"
#include "tokenization.h"
#include "parsing.h"
#include "cpu/cpu_t.h"
#include "translator.h"

//#define _DEBUG

#ifdef _DEBUG
#define DEBUG(code) code
#else
#define DEBUG(code)
#endif

int cur_offset = 0;
int arg_offset = 8;
int if_label = 100;

void translate(struct node_t* node)
{
	FILE* fout = fopen("translate/code_asm.txt", "w");
	assert(fout);
	char *code = calloc(1, (sizeof(char)));
	for (int i = 0; i < name_table_size; i++)
		if (!strcmp("main", name_table[i])) {
			asprintf(&code, "call %d\n", i);
			asprintf(&code, "%spushr rbx\n", code);
			asprintf(&code, "%spushq 1\n", code);
			asprintf(&code, "%spopr rax\n", code);
			asprintf(&code, "%ssyscall\n", code);
			asprintf(&code, "%spushq 0\n", code);
			asprintf(&code, "%spopr rax\n", code);
			asprintf(&code, "%ssyscall\n", code);
		}
	if (!code)
		fputs("Missing main", stderr);
	translate_rec(node, &code);
	assert(code);
	fprintf(fout, code);
	fclose(fout);
	free(code);
}

void translate_rec(struct node_t* node, char** code)
{
	DEBUG(printf("translate_rec: type: %d data: %lf\n", node->type, node->data);)
	if (!node)
		return;
	if (*code == NULL)
			*code = calloc(1, (sizeof(char)));
	switch (node->type) {
	case INT:
			asprintf(code, "%s pushq %d\n", *code, (int)node->data);
			break;
	case DOUBLE:
			asprintf(code, "%s pushq %lf\n", *code, node->data);
			break;
	case TERM:
			translate_term(node, code);
			break;
	case IDENT:
			translate_ident(node, code);
			break;
	case KEYWORD:
			translate_keyword(node, code);
			break;
	default:
			fputs("translate_rec: Unknown node type", stderr);
			break;
	}
}

void translate_ident(struct node_t* node, char** code)
{
	DEBUG(printf("translate_ident: type: %d data: %lf\n", node->type, node->data);)
	if (table_offset[(int)node->data] >= 0)
		asprintf(code, "%s pushmr rbp %d\n", *code, -table_offset[(int)node->data]);
	else {
		translate_rec(node->left, code);
		translate_rec(node->right, code);
		asprintf(code, "%s call %d\n", *code, (int)node->data);
		for (int i = 0; i < -table_offset[(int)node->data] - 1; i++)
			asprintf(code, "%s popr rdi\n", *code);
		asprintf(code, "%s pushr rbx\n", *code);
	}
}

void translate_term(struct node_t* node, char** code)
{
	DEBUG(printf("translate_term: type: %d data: %lf\n", node->type, node->data);)
	switch ((int)node->data) {
	case SEMICOLON:
	case COMMA:
			if (node->left)
				translate_rec(node->left, code);
			if (node->right)
				translate_rec(node->right, code);
			break;
	case BEGINSYM:
			if (node->left)
				translate_rec(node->left, code);
			if (node->right)
				translate_rec(node->right, code);
			break;
	case ENDSYM:
			cur_offset = 0;
			arg_offset = 8;
			if (node->left)
				translate_rec(node->left, code);
			if (node->right)
				translate_rec(node->right, code);
			break;
	case EQL:
			translate_rec(node->left, code);
			translate_rec(node->right, code);
			asprintf(code, "%s jne %d\n", *code, if_label);
			break;
	case LSS:
			translate_rec(node->left, code);
			translate_rec(node->right, code);
			asprintf(code, "%s jge %d\n", *code, if_label);
			break;
	case LEQ:
			translate_rec(node->left, code);
			translate_rec(node->right, code);
			asprintf(code, "%s jg %d\n", *code, if_label);
			break;
	case GTR:
			translate_rec(node->left, code);
			translate_rec(node->right, code);
			asprintf(code, "%s jle %d\n", *code, if_label);
			break;
	case GEQ:
			translate_rec(node->left, code);
			translate_rec(node->right, code);
			asprintf(code, "%s jl %d\n", *code, if_label);
			break;
	case NEQ:
			translate_rec(node->left, code);
			translate_rec(node->right, code);
			asprintf(code, "%s jeq %d\n", *code, if_label);
			break;
	case ADD:
			translate_rec(node->left, code);
			translate_rec(node->right, code);
			asprintf(code, "%s add\n", *code);
			break;
	case SUB:
			translate_rec(node->left, code);
			translate_rec(node->right, code);
			asprintf(code, "%s sub\n", *code);
			break;
	case MUL:
			translate_rec(node->left, code);
			translate_rec(node->right, code);
			asprintf(code, "%s mul\n", *code);
			break;
	case DIV:
			translate_rec(node->left, code);
			translate_rec(node->right, code);
			asprintf(code, "%s div\n", *code);
			break;
	case AND:
	case OR:
			break;
	default:
			fputs("translate_term: Unknown node data\n", stderr);
			break;
	}
}

void translate_keyword(struct node_t* node, char** code)
{
	DEBUG(printf("translate_keyword: type: %d data: %lf\n", node->type, node->data);)
	switch ((int)node->data) {
	case INTSYM:
		{
			int type = -1;
			if (node->right)
				type = node->right->type;
			if(node->left->type == IDENT && type == INT) {
				cur_offset += sizeof(uint64_t);
				asprintf(code, "%s pushq %d\n", *code, (int)node->right->data);
				asprintf(code, "%s popmr rbp %d\n", *code, -cur_offset);
				table_offset[(int)node->left->data] = cur_offset;
			} else
				if(node->left->type == IDENT) {
					asprintf(code, "%s:%d\n", *code, (int)node->left->data);
					asprintf(code, "%s pushr rbp\n", *code);
					asprintf(code, "%s movrr rbp rsp\n", *code);
					int n_arg = 0;
					if (node->right)
						n_arg = translate_arg(node->right, code);
					table_offset[(int)node->left->data] = -1 -n_arg;
				} else {
					fputs("translate_keyword: Syntax error\n", stderr);
				}
			break;
		}
	case DOUBLESYM:
		{
			int type = -1;
			if (node->right)
				type = node->right->type;
			if(node->left->type == IDENT && type == DOUBLE) {
				cur_offset += sizeof(uint64_t);
				asprintf(code, "%s pushq %lf\n", *code, node->right->data);
				asprintf(code, "%s popmr rbp %d\n", *code, -cur_offset);
				table_offset[(int)node->left->data] = cur_offset;
			} else
				if(node->left->type == IDENT) {
					asprintf(code, "%s:%d\n", *code, (int)node->left->data);
					asprintf(code, "%s pushr rbp\n", *code);
					asprintf(code, "%s movrr rbp rsp\n", *code);
					int n_arg = 0;
					if (node->right)
						n_arg = translate_arg(node->right, code);
					table_offset[(int)node->left->data] = -1 -n_arg;
				} else {
					fputs("translate_keyword: Syntax error\n", stderr);
				}
			break;
		}
	case IFSYM:
			translate_rec(node->left, code);
			translate_rec(node->right, code);
			asprintf(code, "%s :%d\n", *code, if_label);
			if_label++;
			break;
	case RETURNSYM:
			translate_rec(node->left, code);
			asprintf(code, "%s popr rbx\n", *code);
			asprintf(code, "%s popr rbp\n", *code);
			asprintf(code, "%s ret\n", *code);
			break;
	default:
			fputs("translate_keyword: Unknown node data\n", stderr);
			break;
	}
}

int translate_arg(struct node_t* node, char** code)
{
	DEBUG(printf("translate_arg: type: %d data: %lf\n", node->type, node->data);)
	switch ((int)node->data) {
	case INTSYM:
		cur_offset += sizeof(uint64_t);
		arg_offset += 8;
		asprintf(code, "%s pushmr rbp %d\n", *code, arg_offset);
		asprintf(code, "%s popmr rsp %d\n", *code, -cur_offset);
		table_offset[(int)node->left->data] = cur_offset;
		if (node->right)
			return translate_arg(node->right, code) + 1;
		break;
	case DOUBLESYM:
		cur_offset += sizeof(uint64_t);
		asprintf(code, "%s pushmr rbp %d\n", *code, arg_offset);
		asprintf(code, "%s popmr rsp %d\n", *code, -cur_offset);
		table_offset[(int)node->left->data] = cur_offset;
		if (node->right)
			return translate_arg(node->right, code) + 1;
		break;
	case COMMA:
		if (node->right)
			return translate_arg(node->right, code);
		if (node->left)
			return translate_arg(node->right, code);
		break;
	default:
		fputs("translate_arg: Unknown node data\n", stderr);
		break;
	}
	return 1;
}
