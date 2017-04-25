// Sugam Garg - 2014A7PS092P, Divish Dayal - 2014A7PS132P

#include<stdio.h>
#include<stdlib.h>
#include"parseTreeDef.h"

extern void push(tree_node **top,int num,char *name);
extern void push_node(tree_node ** top,tree_node * new);
extern void printStack( tree_node *top);
extern tree_node * pop( tree_node **top);
extern int isEmptyStack( tree_node *top);
extern int Top( tree_node *top);
extern void tree_insert_child(tree_node * parent, tree_node * node);
extern void tree_insert_sibling(tree_node * x, tree_node * node);
extern void in_order_traversal(tree_node * node, FILE *fp);
extern tree_node * tree_node_initialise(int x, int line_no, char * value);
