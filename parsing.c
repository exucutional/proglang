/*
 * parsing.c
 *
 *  Created on: 7 ����. 2019 �.
 *      Author: Exucutional
 */

#include <stdio.h>
#include "tokenization.h"
#include "parsing.h"
#include "tree_t.h"

/*
struct node_t* prim(struct list_t* token)
{
	switch (token->type) {
	case INT:
			break;
	case DOUBLE:
			break;
	case TERM:
			break;
	case IDENT:
			return node_create(IDENT, token->data, prim(token->next));
			break;
	case KEYWORD:
			return node_create(KEYWORD, token->data, prim(token->next));
			break;
	default:
			fputs("Unknown list type", stderr);
			break;
	}
}
*/
struct node_t* factor(struct list_t* token)
{
	if (!token->type)
		return NULL;
	switch (token->type) {
	case INT:
	case DOUBLE:
			return muldiv(token->next, node_create(token->type, token->data, NULL, NULL));
	case TERM:
			return term(token, NULL);
	case IDENT:
	case KEYWORD:
			break;
	default:
			fputs("factor: Unknown token type", stderr);
			return NULL;
	}
}

struct node_t* term(struct list_t* token)
{
	switch((int)token->data) {
	case LP:
			return factor(token->next);
			break;
	case RP:
			return;
	default:
			fputs("term: Syntax error", stderr);
			return NULL;
	}
}

struct node_t* muldiv(struct list_t* token, struct node_t* node)
{
	if (token->type != TERM && ((int)token->data != MUL || (int)token->data != DIV))
		return addsub(token, node);
		return node_create(token->type, token->data, node, factor(token->next));
}

struct node_t* addsub(struct list_t* token, struct node_t* node)
{
	if (token->type != TERM && ((int)token->data != ADD || (int)token->data != SUB))
		return node;
		return node_create(token->type, token->data, node, factor(token->next));
}
