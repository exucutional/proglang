/*!
 * \brief tree.h
 * \author Exdev
 * \version 1.0
 */

#ifndef TREE_T_H_
#define TREE_T_H_

#include <stdint.h>

typedef double node_data;
typedef int node_type;

struct node_t
{
	node_type type;
	node_data data;
	struct node_t *left;
	struct node_t *right;
};

struct node_t *node_ctor();
struct node_t *node_create(node_type type, node_data data, struct node_t *left, struct node_t *right);
struct node_t *node_copy(struct node_t *node);

void node_dtor(struct node_t *node);
void node_simple_dump(struct node_t *node);
void node_dump(struct node_t *node);
void dump_rec(struct node_t *node, char **dump_text, int lbl_cur);
void dump_write(struct node_t *node, char**dump_text);


#endif /* TREE_T_H_ */
