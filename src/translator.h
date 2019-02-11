/*
 * translator.h
 *
 *  Created on: 9 февр. 2019 г.
 *      Author: Exucutional
 */

#ifndef SRC_TRANSLATOR_H_
#define SRC_TRANSLATOR_H_

int table_offset[KEY_WORD_MAX + TERMINAL_MAX + IDENT_MAX];
extern int cur_offset;
extern int if_label;
extern int arg_offset;

void translate(struct node_t* node);
void translate_rec(struct node_t* node, char** code);
void translate_term(struct node_t* node, char** code);
void translate_ident(struct node_t* node, char** code);
void translate_keyword(struct node_t* node, char** code);
void translate_arg(struct node_t* node, char** code);

#endif /* SRC_TRANSLATOR_H_ */
