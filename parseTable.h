// Sugam Garg - 2014A7PS092P, Divish Dayal - 2014A7PS132P

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<stdbool.h>
#include"parseTableDef.h"





extern int isUpperCase(const char *inputString);
extern first_node * first_helper(g_node * current, g_node * grammar);
extern first_node * first(int index, g_node * grammar);
extern g_set_node * get_grammar_array(g_node * grammar);
extern first_node * follow(int index, g_node * grammar);
extern void createParseTable(struct table *T,g_node * grammar);
extern g_node * parse_grammar(char * filename, char * elements[]);