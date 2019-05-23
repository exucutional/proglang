/*
 * prog_run.c
 *
 *  Created on: 11 Feb 2019
 *      Author: exucutional
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <sys/stat.h>
#include "tokenization.h"
#include "parsing.h"
#include "tree_t.h"
#include "translator.h"
#include "cpu/cpu_t.h"

const size_t MEMORY_MAX = 1024 * 8120;

char* read_file(FILE *fin);

int main(int argc, char* argv[])
{
	FILE *fin = fopen(argv[1], "r");
	char* code_text = read_file(fin);
	char* code_text_begin = code_text;
	fclose(fin);
	struct token_t* token = token_ctor(str_to_tokens(&code_text));
	//name_table_dump();
	//list_dump(token->iterator);
	struct node_t* node = code(token);
	assert(node);
	//node_dump(node);
	translate(node);
	//fin = fopen("translate/code_asm.txt", "r");
	//char* code_asm_text = read_file(fin);
	//struct stat buff;
	//fstat(fileno(fin), &buff);
	//uint8_t* code_byte = calloc(16 * buff.st_size,sizeof(uint8_t));
	//assert(code_byte);
	//fclose(fin);
	//code_asm(code_asm_text, &code_byte);
	//struct cpu_t cpu = {};
	//cpu_init(&cpu, &code_byte, MEMORY_MAX);
	//fprintf(stderr, "----Running----\n");
	//cpu_run(&cpu);
	node_dtor(node);
	token_dtor(token);
	name_table_dtor();
	//free(code_asm_text);
	free(code_text_begin);
	//free(cpu.mem_min);
	//free(code_byte);
	return 0;
}

char *read_file(FILE *fin)
{
	assert(fin);
	struct stat buff;
	fstat(fileno(fin), &buff);
	char *code = calloc(buff.st_size, sizeof(char));
	fread(code, buff.st_size, 1, fin);
	assert(code);
	return code;
}

