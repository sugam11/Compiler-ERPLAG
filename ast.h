#include<stdio.h>
#include<stdlib.h>

extern void ast_node_delete(tree_node *n);
extern void remove_scrap(tree_node * root);
extern void handle_no_child(tree_node * root);
extern int make_ast(tree_node * root, int i);