/*!
 * \brief tree.c
 * \author Exdev
 * \version 1.2
 */

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "tree_t.h"
#include <ctype.h>

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


struct node_t *node_copy(struct node_t *node)
{
	if (!node)
		return NULL;
	return node_create(node->type, node->data, node_copy(node->left), node_copy(node->right));
}

/*
struct node_t *node_inread(struct node_t *tree, char **intext)
{
	//((1)+(4))
	double tmp = 0;
	assert(*intext);
	while (**intext != '\0') {
		switch(**intext) {
		case '(':
			if (tree == NULL) {
				(*intext)++;
				tree = calloc(1, sizeof(struct node_t));
				break;
			}
			if (tree->left == NULL) {
				tree->left = node_inread(tree->left, intext);
				break;
			}
			tree->right = node_inread(tree->right, intext);
			break;
		case ')':
			(*intext)++;
			return tree;
			break;
		default:
			#define VARIABLE(sym, name)	\
			if (strncmp(*intext, #sym, sizeof(#sym) - 1) == 0) {	\
					tree->data_type = VAR;	\
					tree->val = name;	\
					(*intext) = (*intext) + sizeof(#sym) - 1;	\
					break;	\
				}
			#include "variables.h"
			#undef VARIABLE
			#define OPERATOR(sym, name, act, outform)	\
			if (strncmp(*intext, #sym, sizeof(#sym) - 1) == 0) {	\
				tree->data_type = OPER;	\
				tree->val = name;	\
				(*intext) = (*intext) + sizeof(#sym) - 1;	\
				break;	\
			}
			#include "operators.h"
			#undef OPERATOR
			if (isdigit(**intext)) {
				sscanf(*intext ,"%lf", &tmp);
				if (tmp - (int)tmp != 0)
					tree->data_type = DOUB;
				else
					tree->data_type = INT;
				tree->val = tmp;
				while(isdigit(**intext))
					(*intext)++;
			}
			else {
				(*intext)++;
			}
			break;
		}
	}
	return tree;
}

void node_dtor(struct node_t *tree)
{
	if (tree != NULL) {
		node_dtor(tree->left);
		node_dtor(tree->right);
		free(tree);
	}
}
val_t node_get_data(struct node_t *tree)
{
	assert(tree);
	return(tree->val);
}

val_t node_get_left(struct node_t *tree)
{
	assert(tree);
	return node_get_data(tree->left);
}

val_t node_get_right(struct node_t *tree)
{
	assert(tree);
	return node_get_data(tree->right);
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
	switch(tree->data_type) {
	case INT:
		asprintf(dump_text, "%s%d];\n", *dump_text, (int)tree->val);
		break;
	case DOUB:
		asprintf(dump_text, "%s%lf];\n", *dump_text, tree->val);
		break;
	case VAR:
		switch((int)tree->val) {
		#define VARIABLE(sym, name)	\
		case name:	\
			asprintf(dump_text, "%s\""#sym"\"];\n", *dump_text);	\
			break;
		#include "variables.h"
		#undef VARIABLE
		break;
		default:
			fprintf(stderr, "Unknown variable %d\n", (int)tree->val);
			assert(0);
		}
		break;
	case OPER:
		switch((int)tree->val) {
		#define OPERATOR(sym, name, act, outform)	\
		case name:	\
			asprintf(dump_text, "%s\""#sym"\"];\n", *dump_text);	\
			break;
		#include "operators.h"
		#undef OPERATOR
		default:
			fprintf(stderr, "Unknown operator %d\n", (int)tree->val);
			assert(0);
		}
		break;
	default:
		fprintf(stderr, "Unknown data type %d\n", tree->data_type);
		assert(0);
	}
	#define OPERATOR
}
*/
