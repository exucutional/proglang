/*
 * parsing.c
 *
 *  Created on: 7 февр. 2019 г.
 *      Author: Exucutional
 */

#include <stdio.h>
#include "tokenization.h"


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

struct node_t* factor(struct list_t *token)
{
	if (token->type == INT || token->type == DOUBLE) {
		struct node_t* node = factor(token->next);
		node_create(token->type, token->data, node);
	}
	if (token->type == TERM && token->type == LP) {
		struct node_t* node = factor(token->next);
		if (token->type == TERM && token->type == LP)
		node_create(token->type, token->data);
	}
}
