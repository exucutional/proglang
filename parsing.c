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

#define _DEBUG

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
	if (type == TERM && ((int)data == SEMICOLON)) {
		if (token_get(token)->next)
			return expression(token, node_create(type, data, node, statement(token_next(token))));
		return node_create(type, data, node, NULL);
	}
	if (type == TERM && ((int)data == BEGINSYM)) {
			return node_create(type, data, node, statement(token));
	}
	return node;
}

struct node_t* statement(struct token_t* token)
{
	DEBUG(printf("statement: type: %d data: %lf\n", token_get_type(token), token_get_data(token));)
	switch (token_get_type(token)) {
	case KEYWORD:
		{
			node_data data = token_get_data(token);
			node_type type = token_get_type(token);
			struct node_t* left = statement(token_next(token));
			return node_create(type, data, left, statement(token));
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
	case RP:
	case ENDSYM:
			return NULL;
	default:
			fputs("term_statement: Syntax error(Unknown term type)\n", stderr);
			return NULL;
	}
}
