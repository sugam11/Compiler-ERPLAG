// Sugam Garg - 2014A7PS092P, Divish Dayal - 2014A7PS132P

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<stdbool.h>
#include"lexer.h"
#include"parseTable.h"
#include"parseTree.h"
#include"createParseTree.h"
#include"symbolTable.h"
#include"ast.h"
#define AT 110

void printAST(){
    FILE *fp;
    fp = fopen("ast.txt","r");
    while(1){
        char str[100];
        strcpy(str,getStream(fp));
        printf("%s",str);
        if(strcmp(str,"") == 0){
            break;
        }
    }
}

int count(char *filename){
    FILE *fp;
    fp = fopen(filename,"r");
    int line_no = 0;
    while(1){
        char str[100];
        strcpy(str,getStream(fp));
        line_no++;
        //printf("%s",str);
        if(strcmp(str,"") == 0){
            break;
        }
    }
return line_no;
}

void printTokens(){
    FILE *fp;
    fp = fopen("tokens.txt","r");
    while(1){
        char str[100];
        strcpy(str,getStream(fp));
        printf("%s",str);
        if(strcmp(str,"") == 0){
            break;
        }
    }

}
void printTree(char *filename){
    FILE *fp;
    fp = fopen(filename,"r");
    while(1){
        char str[100];
        strcpy(str,getStream(fp));
        printf("%s",str);
        if(strcmp(str,"") == 0){
            break;
        }
    }


}

int main(int argc,char **argv){

if(argc < 2){
return 0;
}
    struct queue * Q;
    Q = creatQueue();
    
    removeComments(argv[1],"cleanfile.txt");
    
    
    struct firstDataItem *ht = createHashTable(16);
    populateHashTable(ht);
    
    
    char * elements[NO_ELE] = {"program", "moduleDeclarations", "moduleDeclaration", "otherModules", "driverModule", "module", "ret", "input_plist", "N1", "output_plist", "N2", "dataType", "type", "moduleDef", "statements", "statement", "ioStmt", "var", "whichId", "simpleStmt", "assignmentStmt", "whichStmt", "lvalueIDStmt", "lvalueARRStmt", "index", "moduleReuseStmt", "optional", "idlist", "N3", "expression", "arithmeticOrBooleanExpr", "N7", "AnyTerm", "N8", "arithmeticExpr", "N4", "term", "N5", "factor", "op1", "op2", "logicalOp", "relationalOp", "declareStmt", "condionalStmt", "caseStmts", "N9", "value", "default", "iterativeStmt", "range",
                                "RANGEOP", "WHILE", "IN", "FOR", "DEFAULT", "FALSE", "TRUE", "BREAK", "CASE", "SWITCH", "NE", "EQ", "GE", "GT", "LE", "LT", "OR", "AND", "DIV", "MUL", "PLUS", "MINUS", "PARAMETERS", "WITH", "USE", "ASSIGNOP", "RNUM", "NUM", "PRINT", "BC", "BO", "END", "START", "OF", "ARRAY", "BOOLEAN", "REAL", "INTEGER", "COMMA", "COLON", "RETURNS", "SQBC", "SQBO", "INPUT", "TAKES", "ENDDEF", "DEF", "DRIVERENDDEF", "PROGRAM", "DRIVER", "DRIVERDEF", "SEMICOL", "ID", "MODULE", "DECLARE", "EPSILON", "GET_VALUE", "|", "$"};
    

    
    char * filename = "grammar.txt";
    g_node * grammar = parse_grammar(filename, elements);
    
    FILE * token_fp = fopen("tokens.txt","w");
    
    struct table *T;
    T = (struct table *)malloc(sizeof(struct table));
    createParseTable(T,grammar);
    
    generateTokens(Q,ht,token_fp);
    
    tree_node * root = createTree(grammar,T,Q,elements,"parseTree.txt");

    //printStack(top);
    FILE * fp = fopen("ast.txt", "w");
    //fputs("AST\n",fp);

    //ast
    root->parent = NULL;
    remove_scrap(root);
    //in_order_traversal(root,fp);
    for(int i = 0; i<8; i++)
        make_ast(root, i);
    
    handle_no_child(root);



    in_order_traversal(root,fp);
    


    fclose(fp);

    struct symbolTable *ST = NULL;
    ST = createSymbolTable(ST);

    /*for(int i = 0 ; i < NO_NON_TERM;i++){
                  for (int j = 0; j < NO_TERM + 1; j++) {
                      if(strcmp(T->cell[i][j].rule,"") != 0)
                          printf("the rule written at [%d][%d] is : %s\n",i,j,T->cell[i][j].rule);
                  }
              }*/
    
    //struct symbolTable *ST = NULL;
    //createSymbolTable(ST);
    int AST_nodes = count("ast.txt");
    int parseTree_nodes = count("parseTree.txt");

    int AST_size = AST_nodes * sizeof(tree_node);
    int parseTree_size = parseTree_nodes * sizeof(tree_node);
    float compression = (((float)parseTree_size - (float)AST_size)/(float)parseTree_size)*100;

    printf("\n\n\n\n\n\n\nLEVEL 2 : symbolTable/AST/Semantic rules modules work\n");
    printf("Both lexical and syntax analysis modules implemented\n");
    printf("Semantic Errors and type checking implemented in symbolTable\n");
    printf("Please use the grammar in the zip file as grammar.txt, modifying it may lead to unpredicatble results\n\n\n");
    
    while(1){
        
        
        printf("Press 1: For printing the token list\n");
        printf("Press 2 : For parsing to verify the syntactic correctness of the input source code and printing parsetree\n");
        printf("Press 3 : For printing the Abstract Syntax tree\n");
        printf("Press 4 : For displaying memory allocation\n");
        printf("Press 5 : For printing symbol table\n");
        printf("Press 9 : Exit\n");
        int input;
        scanf("%d",&input);
        if(input == 9){
            break;
        }
        switch (input) {
            case 1:
                //fp = fopen("tokens.txt","w");
                printTokens();
                break;
            case 2 :
                printTree("parseTree.txt");
                break;
            case 3 :
                printf("Inorder printing\n");
                printAST();
                break;
            case 4 : 
                printf("Parse tree     Number of nodes = %d  Allocated memory = %d\n",parseTree_nodes,parseTree_size);
                printf("AST     Number of nodes = %d  Allocated memory = %d\n",AST_nodes,AST_size);
                printf("Compression percentage = %f\n",compression );
                break;
            case 5 : 

                print_ST(ST);
                break;
        }
    }
    
    
}