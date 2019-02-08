/*
 * tokenization.c
 *
 *  Created on: 5 февр. 2019 г.
 *      Author: Exucutional
 */

#include "tokenization.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>

int name_table_size = 0;

struct list_t* list_create(list_data data, list_type type, struct list_t* next)
{
	struct list_t* list = calloc(1, sizeof(struct list_t));
	assert(list);
	list->data = data;
	list->type = type;
	list->next = next;
	return list;
}

void list_dtor(struct list_t* list)
{
	if (list) {
		list_dtor(list->next);
		free(list);
	}
}

void list_dump(struct list_t* list)
{
	static int dump_flag;
	if (dump_flag == 0) {
		printf("LIST DUMP:\n");
		dump_flag++;
	}
	if(list) {
		printf("%lf %d -> ", list->data, list->type);
		list_dump(list->next);
	} else printf("NULL");
}

void name_table_dump()
{
	printf("NAME TABLE DUMP:\n");
	for(int i = 0; i < name_table_size; i++)
		printf("%d) %s\n", i, name_table[i]);
}

void name_table_dtor()
{
	for(int i = 0; i < name_table_size; i++)
			free(name_table[i]);
	name_table_size = 0;
}

struct list_t* str_to_tokens(char** str_ptr)
{
	char* token = get_token(str_ptr);
	if (**str_ptr == '\0' && !token)
		return NULL;
	if (!token)
		return  list_num(str_ptr);
#define DEF_KEYWORD(sym, name)																	\
	do {																						\
			if (strcmp(token, #sym) == 0) {														\
				*str_ptr += sizeof(#sym) - 1;													\
				free(token);																	\
				return list_create(name, KEYWORD, str_to_tokens(str_ptr));						\
			}																					\
} while (0);
#include "keywords.h"
#undef DEF_KEYWORD

#define DEF_TERM(sym, name)																	\
	do {																					\
			if (strcmp(token, #sym) == 0) {													\
				*str_ptr += sizeof(#sym) - 1;												\
				free(token);																\
				return list_create(name, TERM, str_to_tokens(str_ptr));						\
			}																				\
	} while (0);
#include "terminals.h"
#undef DEF_TERM

	struct list_t* list = list_create(name_table_search(token), IDENT, str_to_tokens(str_ptr));
	free(token);
	return list;
}


int name_table_search(const char* name)
{
	int i = 0;
	for (; i < name_table_size; i++)
		if (!strcmp(name_table[i], name))
			return i;
	name_table_size = i + 1;
	name_table[i] = strdup(name);
	return i;
}

struct list_t* list_num(char** str_ptr)
{
	char* iter = *str_ptr;
	int dot = 0;
	for (;isdigit(*iter) || *iter == '.'; iter++)
		if (*iter == '.')
			dot++;
	if (dot == 0) {
		long inttmp = strtol(*str_ptr, str_ptr, 0);
		return list_create(inttmp, INT, str_to_tokens(str_ptr));
	}
	if (dot == 1) {
		double doubtmp = strtod(*str_ptr, str_ptr);
		return list_create(doubtmp, DOUBLE, str_to_tokens(str_ptr));
	}
	fputs("ERROR: dots > 1", stderr);
	return NULL;
}

char* get_token(char** str_ptr)
{
	while(isspace(**str_ptr))
		(*str_ptr)++;
	if (**str_ptr == '\0')
		return NULL;
	if (isdigit(**str_ptr))
		return NULL;
	int token_length = 0;
	char* iter = *str_ptr;
	while ((!isspace(*iter) && *iter != '\0') ||
	#define DEF_TERM(sym, name)	!strncmp(iter, #sym, sizeof(#sym) - 1) ||
	#include "terminals.h"
	#undef DEF_TERM
			0) {
		token_length++;
		iter++;
	}
	char* token = calloc(token_length + 1, sizeof(char));
	token = strncpy(token, *str_ptr, token_length);
	token[token_length] = '\0';

	*str_ptr += token_length;
	return token;
}
