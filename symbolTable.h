// Sugam Garg - 2014A7PS092P, Divish Dayal - 2014A7PS132P

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<stdbool.h>
#include"symbolTableDef.h"
#include"parseTreeDef.h"



extern int moduleSearch(char *module_name,struct symbolTable *ST);
extern struct symbolTable * createSymbolTable(struct symbolTable *ST);
extern int search_ST(struct file_read * curr,struct symbolTable *ST,int scope,bool declare_flag,bool module_flag);
extern char * symbol_type(char *symbol_name,int line_no,struct symbolTable *ST);
extern void print_ST(struct symbolTable *ST);