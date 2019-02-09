/*
 * unit_test.c
 *
 *  Created on: 6 февр. 2019 г.
 *      Author: Exucutional
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "tokenization.h"
#include "parsing.h"
#include "tree_t.h"

int main()
{
	char **input = calloc(1,sizeof(char*));
	*input = strdup(
			"int main(int a) {"
			"int x = 3;"
			"int b = 45;"
			"int c = 34;"
			"}");

	struct token_t* token = token_ctor(str_to_tokens(input));
	assert(token);
	name_table_dump();
	list_dump(token->iterator);

	struct node_t* node = code(token);
	assert(node);
	node_dump(node);

	node_dtor(node);
	token_dtor(token);
	name_table_dtor();
	return 0;
}


