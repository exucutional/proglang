/*
 * tokenization.h
 *
 *  Created on: 5 ����. 2019 �.
 *      Author: Exucutional
 */

#ifndef TOKENIZATION_H_
#define TOKENIZATION_H_

enum KEYWORDS {
#define DEF_KEYWORD(sym, name)	\
	name,
#include "keywords.h"
	KEY_WORD_MAX,
#undef DEF_KEYWORD
};

enum TERMINALS {
#define DEF_TERM(sym, name)	\
	name,
#include "terminals.h"
	TERMINAL_MAX,
#undef DEF_TERM
};

enum TOKEN_TYPES {
	NO_TYPE = 0,	//0
	INT,			//1
	DOUBLE,			//2
	TERM,			//3
	IDENT,			//4
	KEYWORD,		//5
};



#define IDENT_MAX 100
char *name_table[KEY_WORD_MAX + TERMINAL_MAX + IDENT_MAX];
extern int name_table_size;

typedef double list_data;
typedef int list_type;

struct list_t
{
	list_data data;
	list_type type;
	struct list_t* next;
};

struct token_t
{
	struct list_t* iterator;
	struct list_t* token_list;
};

struct list_t* list_create(list_data data, list_type type, struct list_t* next);
struct list_t* str_to_tokens(char** str_ptr);
struct list_t* list_num(char** str_ptr);
struct token_t* token_ctor(struct list_t* token_list);
struct list_t* token_get(struct token_t* token);
list_data token_get_data(struct token_t* token);
list_type token_get_type(struct token_t* token);
struct list_t* token_pop(struct token_t* token);
struct token_t* token_next(struct token_t* token);
int name_table_search(const char* name);
char* token_read(char** str_ptr);
void name_table_dump();
void name_table_dtor();
void list_dump(struct list_t* list);
void list_dtor(struct list_t* list);
void token_dtor(struct token_t* token);
#endif /* TOKENIZATION_H_ */
