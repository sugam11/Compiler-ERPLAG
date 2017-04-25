// Sugam Garg - 2014A7PS092P, Divish Dayal - 2014A7PS132P

#ifndef _parseTreeDef
#define _parseTreeDef
typedef struct tree_node{
    int num;
    char name[50];
    int line_no;
    char value[50];
    struct tree_node * lchild;
    struct tree_node * rchild;
    struct tree_node * parent;
    struct tree_node * next;
    //int visit; // 0 for unvisited; 1 for visited; 2 for deleted
}tree_node;


typedef struct stack_node{
    int num;
    char name[20];
    struct stack_node * next;
    tree_node * node_tree;
} stack_node;


#endif
