/*
 * unit_test.c
 *
 *  Created on: 6 февр. 2019 г.
 *      Author: Exucutional
 */

#include "tokenization.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

int main()
{
	char **input = calloc(1,sizeof(char*));
	*input = strdup("Hello i am 98 years old");

	struct list_t* tokens = str_to_tokens(input);
	assert(tokens);
	name_table_dump();
	list_dump(tokens);
	list_dtor(tokens);
	name_table_dtor();

	return 0;
}


