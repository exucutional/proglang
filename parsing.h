/*
 * parsing.h
 *
 *  Created on: 7 ����. 2019 �.
 *      Author: Exucutional
 */

#ifndef PARSING_H_
#define PARSING_H_

//int x = (56 + 3) * 6;

struct node_t* factor(struct list_t* token);
struct node_t* term(struct list_t* token);
struct node_t* muldiv(struct list_t* token, struct node_t* node);
struct node_t* addsub(struct list_t* token, struct node_t* node);

/*
struct node_t* condition(void);
struct node_t* term(struct list_t* token);
struct node_t* prim(struct list_t* token);
struct node_t* statement(void);
struct node_t* expression(void);
struct node_t* block(void);
*/
#endif /* PARSING_H_ */
