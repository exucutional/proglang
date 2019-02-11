/*
 * parsing.c
 *
 *  Created on: 7 ����. 2019 �.
 *      Author: Exucutional
 */

#include <stdio.h>

#include "tree_t.h"
#include "tokenization.h"
#include "parsing.h"

//#define _DEBUG

#ifdef _DEBUG
#define DEBUG(code) code
#else
#define DEBUG(code)
#endif

struct node_t* calculation(struct token_t* token)
{
	DEBUG(printf("calculation: type: %d data: %lf\n", token_get_type(token), token_get_data(token));)
	return addsub(token, muldiv(token, factor(token)));
}

struct node_t* factor(struct token_t* token)
{
	DEBUG(printf("factor: type: %d data: %lf\n", token_get_type(token), token_get_data(token));)
	switch (token_get_type(token)) {
	case INT:
	case DOUBLE:
	case IDENT:
		{
			node_data data = token_get_data(token);
			node_type type = token_get_type(token);
			if (token_get(token)->next)	{
				token_next(token);
			}
			if (token_get_type(token) == TERM && token_get_data(token) == LP) {
				struct node_t* left = calculation(token_next(token));
				struct node_t* right = node_create(type, data, left, calculation(token));
				if (token_get_data(token) == RP) {
					token_next(token);
					return right;
				} else {
					fputs("factor: Missing right parenthesis\n", stderr);
					return right;
				}
			} else {
				return node_create(type, data, NULL, NULL);
			}
			return NULL;
		}
	case TERM:
			return term_calc(token);
	default:
			fputs("factor: Unknown token type\n", stderr);
			return NULL;
	}
}

struct node_t* term_calc(struct token_t* token)
{
	DEBUG(printf("term_calc: type: %d data: %lf\n", token_get_type(token), token_get_data(token));)
	switch((int)token_get_data(token)) {
	case LP:
		{
			struct node_t* node = calculation(token_next(token));
			if ((int)token_get_data(token) == RP)
			{
				if (token_get(token)->next)
					token_next(token);
				return node;
			}
			fputs("term_calc: Syntax error(Missing right parenthesis)\n", stderr);
			return NULL;
		}
	case COMMA:
		{
			node_data data = token_get_data(token);
			node_type type = token_get_type(token);
			struct node_t* left = calculation(token_next(token));
			return node_create(type, data, left, calculation(token));
		}
	case RP:
			return NULL;
	default:
			fputs("term_calc: Syntax error(Unknown term type)\n", stderr);
			return NULL;
	}
}

struct node_t* muldiv(struct token_t* token, struct node_t* node)
{
	DEBUG(printf("muldiv: type: %d data: %lf\n", token_get_type(token), token_get_data(token));)
	node_data data = token_get_data(token);
	node_type type = token_get_type(token);
	if (type == TERM && ((int)data == MUL || (int)data == DIV))
		return muldiv(token, node_create(type, data, node, factor(token_next(token))));
	return node;
}

struct node_t* addsub(struct token_t* token, struct node_t* node)
{
	DEBUG(printf("addsub: type: %d data: %lf\n", token_get_type(token), token_get_data(token));)
	node_data data = token_get_data(token);
	node_type type = token_get_type(token);
	if (type == TERM && ((int)data == ADD || (int)data == SUB))
		return addsub(token, node_create(type, data, node, muldiv(token, factor(token_next(token)))));
	return node;
}

struct node_t* code(struct token_t* token)
{
	DEBUG(printf("code: type: %d data: %lf\n", token_get_type(token), token_get_data(token));)
	return expression(token, statement(token));
}

struct node_t* expression(struct token_t* token, struct node_t* node)
{
	DEBUG(printf("expression: type: %d data: %lf\n", token_get_type(token), token_get_data(token));)
	node_data data = token_get_data(token);
	node_type type = token_get_type(token);
	if (type == TERM && ((int)data == SEMICOLON || (int)data == COMMA)) {
		if (token_get(token)->next)
			return expression(token, node_create(type, data, node, statement(token_next(token))));
		return node_create(type, data, node, NULL);
	}
	if (type == TERM && ((int)data == BEGINSYM)) {
			struct node_t* left = node_create(type, data, node, code(token_next(token)));
			if ((int)token_get_data(token) == ENDSYM) {
				if (token_get(token)->next)
					return node_create(token_get_type(token), token_get_data(token), left, code(token_next(token)));
				return node_create(token_get_type(token), token_get_data(token), left, NULL);
			}
			fputs("expression: Missing ENDSYM\n", stderr);
			return left;
	}
	return node;
}

struct node_t* statement(struct token_t* token)
{
	DEBUG(printf("statement: type: %d data: %lf\n", token_get_type(token), token_get_data(token));)
	switch (token_get_type(token)) {
	case KEYWORD:
		{
			return keyword_statement(token);
		}
	case IDENT:
		{
			node_data data = token_get_data(token);
			node_type type = token_get_type(token);
			token_next(token);
			return node_create(type, data, NULL, NULL);
		}
	case TERM:
			return term_statement(token);
	default:
		fputs("statement: Syntax error(Unknown token type)\n", stderr);
		return NULL;
	}
}

struct node_t* term_statement(struct token_t* token)
{
	DEBUG(printf("term_statement: type: %d data: %lf\n", token_get_type(token), token_get_data(token));)
	switch ((int)token_get_data(token)) {
	case LP:
		{
			struct node_t* node = statement(token_next(token));
			if (token_get_data(token) == RP) {
				token_next(token);
				return node;
			}
			fputs("term_calc: Syntax error(Missing right parenthesis)\n", stderr);
			return NULL;
		}
	case EQ:
			return calculation(token_next(token));
	case BEGINSYM:
		{
			struct node_t* node = code(token_next(token));
			if ((int)token_get_data(token) == ENDSYM)	{
				if (token_get(token)->next)
					token_next(token);
				return node;
			}
			fputs("term_statement: Syntax error(Missing endsym)\n", stderr);
			return NULL;
		}
	case COMMA:
		{
			//struct node_t* left = statement(token_next(token));
			//return node_create(TERM, COMMA, left, statement(token));
			return expression(token, NULL);
		}
	case RP:
	case ENDSYM:
			return NULL;
	default:
			fputs("term_statement: Syntax error(Unknown term type)\n", stderr);
			return NULL;
	}
}

struct node_t* keyword_statement(struct token_t* token)
{
	DEBUG(printf("keyword_statement: type: %d data: %lf\n", token_get_type(token), token_get_data(token));)
	switch ((int)token_get_data(token)) {
	case INTSYM:
	case DOUBLESYM:
		{
			node_data data = token_get_data(token);
			node_type type = token_get_type(token);
			struct node_t* left = statement(token_next(token));
			return node_create(type, data, left, statement(token));
		}
	case IFSYM:
		{
			node_data data = token_get_data(token);
			node_type type = token_get_type(token);
			struct node_t* left = condition(token_next(token));
			if ((int)token_get_data(token) == RP)
				return expression(token, node_create(type, data, left, statement(token_next(token))));
			fputs("keyword_statement: Syntax error(Missing right parenthesis)\n", stderr);
			return left;
		}
	case RETURNSYM:
		{
			node_data data = token_get_data(token);
			node_type type = token_get_type(token);
			struct node_t* left = calculation(token_next(token));
			return node_create(type, data, left, NULL);
		}
	default:
		fputs("keyword_statement: Syntax error(Unknown keyword type)\n", stderr);
		return NULL;
	}
}
struct node_t* condition(struct token_t* token)
{
	node_data data = token_get_data(token);
	node_type type = token_get_type(token);
	DEBUG(printf("condition: type: %d data: %lf\n", type, data);)
	if (type == TERM && data == LP) {
		struct node_t* left = calculation(token_next(token));
		data = token_get_data(token);
		type = token_get_type(token);
		switch((int)data) {
		case EQL:
		case LSS:
		case LEQ:
		case GTR:
		case GEQ:
		case AND:
		case OR:
		case NEQ:
			return node_create(type ,data, left, calculation(token_next(token)));
		default:
			fputs("condition: Syntax error(Unknown term type)\n", stderr);
			return NULL;
		}
	}
	fputs("conditon: Syntax error(Unknown condition type)\n", stderr);
	return NULL;
}
