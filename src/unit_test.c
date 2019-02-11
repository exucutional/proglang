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
#include "translator.h"
#include "cpu/cpu_t.h"

const size_t MEMORY_SIZE = 4096;

int main()
{
	char **input = calloc(1,sizeof(char*));
	 *input = strdup(
			"int f(int a) {"
			"return a + 3;"
			"} "
			"int main() {"
			"return f(4);"
			"}"
			);

	struct token_t* token = token_ctor(str_to_tokens(input));
	assert(token);
	//name_table_dump();
	//list_dump(token->iterator);
	struct node_t* node = code(token);
	assert(node);

	//translate(node);

	char *text = calloc(MEMORY_SIZE, sizeof(char));
	uint8_t *code = calloc(MEMORY_SIZE, sizeof(uint8_t));
	FILE *fin = NULL;
	fin = fopen("translate/code.txt", "r");
	assert(fin);
	fread(text, 1024, 1, fin);
	code_asm(text, &code);
	struct cpu_t cpu = {};
	cpu_init(&cpu, &code);
	fprintf(stderr, "----Running----\n");
	cpu_run(&cpu);

	node_dump(node);
	node_dtor(node);
	token_dtor(token);
	name_table_dtor();
	free(*input);
	free (code);
	free (text);
	return 0;
}


