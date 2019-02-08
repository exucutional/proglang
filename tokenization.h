/*
 * tokenization.h
 *
 *  Created on: 5 февр. 2019 г.
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
	NO_TYPE = 0,
	INT,
	DOUBLE,
	TERM,
	IDENT,
	KEYWORD,
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

struct list_t* list_create(list_data data, list_type type, struct list_t* next);
struct list_t* str_to_tokens(char** str_ptr);
struct list_t* list_num(char** str_ptr);
int name_table_search(const char* name);
char* get_token(char** str_ptr);
void name_table_dump();
void name_table_dtor();
void list_dump(struct list_t* list);
void list_dtor(struct list_t* list);

#endif /* TOKENIZATION_H_ */
