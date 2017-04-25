// Sugam Garg - 2014A7PS092P, Divish Dayal - 2014A7PS132P

#ifndef _symbolTableDef
#define _symbolTableDef

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>


struct table_row{
    int s_no;
    char lexeme[15];
    char type[20];
    char scope[20];
    int startScope;
    int endScope;
    int scopeNesting;
    int id_width;
    int offset;
    int line_no;
};


struct symbolTable{
    struct table_row row[100];
    int row_num;
    char module_name[10];
    struct symbolTable * next;
    struct symbolTable * prev;
};

struct file_read{
    int line_no;
    int token_no;
    char lexeme[20];
    char token[20];
};

#endif
