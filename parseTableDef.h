// Sugam Garg - 2014A7PS092P, Divish Dayal - 2014A7PS132P

#ifndef _parseTableDef
#define _parseTableDef

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>


#define MAX_RULES  51
#define NO_ELE  110
#define NO_NON_TERM  51
#define MAX_FIRST_SET 20
#define EPS 106
#define OR_RULE 108
#define PROGRAM_INDEX 0
#define DOLLAR_INDEX 109
#define NO_TERM 57
#define RULE_INDEX 17
#define NON_TERM_INDEX1 69
#define NON_TERM_INDEX2 70

//char * elements[NO_ELE];

struct tableCell{
    char rule[100];
};

struct table{
    struct tableCell cell[NO_NON_TERM][NO_TERM + 1];            //Plus 1 for $ column at end
};


////////////////////////structure for FIRST/ FOLLOW grammar set /////////////////
typedef struct first_node{
    int index;
    bool eps;
    struct first_node * next;
}first_node;




////////////////////////structure for grammar PARSING DATA STRUCTURE set /////////////////

typedef struct g_set_node{
    int line;
    struct g_set_node * next;
}g_set_node;





typedef enum
{ term=0, non_term=1} g_type;

///////////////////////// structure for grammar array ---------------------------
struct g_node
{
    int g_no;
    char name[30];
    g_type type;
    struct g_node * next;
};
typedef struct g_node g_node;

first_node * first(int index, g_node * grammar);


#endif
