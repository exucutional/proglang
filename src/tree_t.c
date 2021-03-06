/*!
 * \brief tree.c
 * \author Exdev
 * \version 1.2
 */
//#include "asprintf.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include "tree_t.h"

#include "tokenization.h"

#define NODE_CH_WRT(type, name)	\
	do {	\
		tree->data = strdup(name);	\
		tree->data_type = type;	\
	} while (0)

struct node_t *node_ctor()
{
	struct node_t *tree = calloc(1, sizeof(struct node_t));
	return tree;
}

struct node_t *node_create(node_type type, node_data data, struct node_t *left, struct node_t *right)
{
	struct node_t *node = calloc(1, sizeof(struct node_t));
	node->type = type;
	node->data = data;
	node->left = left;
	node->right = right;
	return node;
}


struct node_t *node_copy(struct node_t* node)
{
	if (!node)
		return NULL;
	return node_create(node->type, node->data, node_copy(node->left), node_copy(node->right));
}

void node_dtor(struct node_t* node)
{
	if (!node) {
		node_dtor(node->left);
		node_dtor(node->right);
		free(node);
	}
}
void node_simple_dump(struct node_t* node)
{
	static int nodes = 0;
	if (!nodes)
		printf("NODE SIMPLE DUMP\n");
	if(!node)
		return;
	printf("%d) type: %d data: %lf\n", nodes, node->type, node->data);
	nodes++;
	node_simple_dump(node->left);
	node_simple_dump(node->right);
}

void node_dump(struct node_t *tree)
{
	FILE *fout = fopen("tree.dot", "w");
	assert(fout);
	fprintf(fout,"digraph tree {\n");
	char *dump_text = NULL;
	dump_rec(tree, &dump_text, 0);
	assert(dump_text);
	fprintf(fout, dump_text);
	fprintf(fout,"}");
	fclose(fout);
	free(dump_text);
	system("dot -Tpng tree.dot -o tree.png");
	system("xdg-open tree.png");
}

void dump_rec(struct node_t *tree, char **dump_text, int lbl_cur)
{
	static int lbl_max;
	if (!tree)
		return;
	if (*dump_text == NULL)
		*dump_text = calloc(1, (sizeof(char)));
	if(tree->left != NULL || tree->right != NULL) {
		asprintf(dump_text, "%s%d [label=", *dump_text, lbl_cur);
		dump_write(tree, dump_text);
		if (tree->left != NULL) {
			asprintf(dump_text, "%s%d [label=", *dump_text, lbl_max + 1);
			dump_write(tree->left, dump_text);
			asprintf(dump_text, "%s%d -> %d;\n", *dump_text, lbl_cur, lbl_max + 1);
		}
		if (tree->right != NULL) {
			asprintf(dump_text, "%s%d [label=", *dump_text, lbl_max + 2);
			dump_write(tree->right, dump_text);
			asprintf(dump_text, "%s%d -> %d;\n", *dump_text, lbl_cur, lbl_max + 2);
		}
		lbl_cur = lbl_max;
		lbl_max += 3;
		dump_rec(tree->left, dump_text, lbl_cur + 1);
		dump_rec(tree->right, dump_text, lbl_cur + 2);
	}
}

void dump_write(struct node_t *tree, char**dump_text)
{
	switch(tree->type) {
	case INT:
		asprintf(dump_text, "%s%d];\n", *dump_text, (int)tree->data);
		break;
	case DOUBLE:
		asprintf(dump_text, "%s%lf];\n", *dump_text, tree->data);
		break;
	case IDENT:
			asprintf(dump_text, "%s\"%s\"];\n", *dump_text, name_table[(int)tree->data]);
		break;
	case KEYWORD:
		switch((int)tree->data) {
		#define DEF_KEYWORD(sym, name)								\
		case name:													\
			asprintf(dump_text, "%s\""sym"\"];\n", *dump_text);		\
			break;
		#include "keywords.h"
		#undef DEF_KEYWORD
		default:
			fprintf(stderr, "Unknown keyword %d\n", (int)tree->data);
			assert(0);
		}
		break;
	case TERM:
		switch((int)tree->data) {
		#define DEF_TERM(sym, name)									\
		case name:													\
			asprintf(dump_text, "%s\""sym"\"];\n", *dump_text);		\
			break;
		#include "terminals.h"
		#undef DEF_TERM
		default:
			fprintf(stderr, "Unknown terminal %d\n", (int)tree->data);
			assert(0);
		}
		break;
	default:
		fprintf(stderr, "Unknown data type %d\n", tree->type);
		assert(0);
	}
}
