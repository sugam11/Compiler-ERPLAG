// Sugam Garg - 2014A7PS092P, Divish Dayal - 2014A7PS132P

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<stdbool.h>
#include"parseTableDef.h"



// //char * elements[NO_ELE];

// struct tableCell{
//     char rule[100];
// };

// struct table{
//     struct tableCell cell[NO_NON_TERM][NO_TERM + 1];            //Plus 1 for $ column at end
// };


// ////////////////////////structure for FIRST/ FOLLOW grammar set /////////////////
// typedef struct first_node{
//     int index;
//     bool eps;
//     struct first_node * next;
// }first_node;




// ////////////////////////structure for grammar PARSING DATA STRUCTURE set /////////////////

// typedef struct g_set_node{
//     int line;
//     struct g_set_node * next;
// }g_set_node;





// typedef enum
// { term=0, non_term=1} g_type;

// ///////////////////////// structure for grammar array ---------------------------
// struct g_node
// {
//     int g_no;
//     char name[30];
//     g_type type;
//     struct g_node * next;
// };
// typedef struct g_node g_node;

// first_node * first(int index, g_node * grammar);

int isUpperCase(const char *inputString)
{
    int i;
    int len = strlen(inputString);
    for (i = 0; i < len; i++) {
        if (isupper(inputString[i]) == 0) {
            if(inputString[i] == '\n' || inputString[i] == '_')
                continue;
            return 0;
        }
    }
    return 1;
}



////////////////////////////// -------   FIRST -------   \\\\\\\\\\\\\////////////////////

first_node * first_helper(g_node * current, g_node * grammar){
    //g_node * current = grammar[index].next;
    //printf("%s \n", current->name);
    //printf("helper calling first : %s. \n", current->name);
    first_node * current_first_set = first(current->g_no, grammar);
    first_node * head = current_first_set;
    
    while(head->eps == true && current->next != NULL && current->next->g_no != OR_RULE){
        current = current->next;
        //printf("inside loop :helper calling first : %s. \n", current->name);
        current_first_set = first(current->g_no, grammar);
        first_node * last_first_set;
        last_first_set = current_first_set;
        while(last_first_set->next != NULL){
            last_first_set = last_first_set->next;
        }
        last_first_set->next = head;
        
        head = current_first_set;
        
    }
    
    
    
    
    return head;
}



first_node * first(int index, g_node * grammar){
    
    first_node * head = (first_node*)malloc(sizeof(first_node));
    head->eps = false;
    head->next = NULL;
    
    //if terminal, return index
    if(index >=MAX_RULES){
        head->index = index;
        return head;
    }
    
    
    
    //if X->EPSILON, add it.
    g_node * eps_check = grammar[index].next->next;
    while(eps_check != NULL){
        if(eps_check->g_no == EPS)
            head->eps = true;
        eps_check = eps_check->next;
    }
    int flag_eps = 0;
    if(head->eps == true)
        flag_eps = 1;
    
    //adding for all those who have epsilon as or.
    g_node * current = grammar[index].next;
    // first_node * current_first_set = first(current->g_no, grammar);
    // head = current_first_set;
    
    // while(head->eps == true && current->next != NULL){
    // 	current = current->next;
    // 	current_first_set = first(current->g_no, grammar);
    // 	first_node * last_first_set = current_first_set;
    // 	while(last_first_set->next != NULL){
    // 		last_first_set = last_first_set->next;
    // 	}
    // 	last_first_set->next = head;
    // 	head = current_first_set;
    // }
    //printf("first calling helper : %s. \n", current->name);
    head = first_helper(current, grammar);
    
    
    
    //correcting eps property of head i.e. final firt set.
    if(head->eps == true)
        flag_eps = 1;
    if(flag_eps == 1)
        head->eps = true;
    else
        head->eps = false;
    
    
    //adding for or conditions
    current = current->next;
    while(current != NULL){
        
        if(current->g_no == OR_RULE){
            first_node * head_new;
            current = current->next;
            if(current->g_no == EPS){
                current = current->next;
                continue;
            }
            head_new = first_helper(current, grammar);
            first_node * temp = head_new;
            while(head_new->next != NULL)
                head_new = head_new->next;
            head_new->next = head;
            head = temp;
            
            
            
            if(head->eps == true)
                flag_eps = 1;
            if(flag_eps == 1)
                head->eps = true;
            else
                head->eps = false;
        }
        
        current = current->next;
    }
    
    return head;
    
    
}


///////////////////// First END /////////////////////// /////////////////////// /////////////////////// ///////////////////////



///////////////////// GET_GRAMMAR ARRAY /////////////////////// /////////////////////// /////////////////////// ////////////////
//for getting the array with indices as non terminals and array elements as linked list of g_set_node datastructure containing line numbers
//of grammar rules where the non terminal exists on the rhs.
g_set_node * get_grammar_array(g_node * grammar){
    g_set_node * arr = (g_set_node *)malloc(NO_ELE* sizeof(g_set_node));
    for(int i = 0;i<NO_ELE;i++){
        arr[i].line =-1;
        arr[i].next=NULL;
    }
    //arr[] =
    for(int i = 0; i < MAX_RULES; i++){
        g_node * current;
        current = grammar[i].next;
        while(current != NULL){
            int x = current->g_no;
            if(current->name[0] == '|')
                x = 108;
            
            if(arr[x].line == -1)
                arr[x].line = i;
            else{
                g_set_node * new = (g_set_node*)malloc(sizeof(g_set_node));
                new->line = i;
                new->next = NULL;
                g_set_node * temp = arr[x].next;
                if( temp == NULL)
                    arr[x].next = new;
                else{
                    while(temp->next != NULL){
                        //printf("In heeree\n");
                        temp = temp->next;
                    }
                    temp->next = new;
                }
                //printf("%d \n",temp.next->line);
            }
            current = current->next;
            
        }
    }
    
    return arr;
}





////////////////////////////////////////// FOLLOW //////////////////////////////////////////////////////////////////////



first_node * follow(int index, g_node * grammar){
    first_node * head = (first_node*)malloc(sizeof(first_node));
    head->next = NULL;
    head->index = -1;
    int flag_line = 0;
    
    //nonterminal case
    if(index >= NO_NON_TERM){
        head->index = index;
        return head;
    }else if(index == PROGRAM_INDEX){
        head->index = DOLLAR_INDEX;
        return head;
    }
    g_set_node * g_set = get_grammar_array(grammar);
    g_set_node * g_set_index = &g_set[index];
    
    while(g_set_index != NULL){
        int line = g_set_index->line;
        g_node * grammar_line;
        grammar_line = grammar[line].next;
        
        while(grammar_line != NULL && grammar_line->g_no != index){
            grammar_line = grammar_line->next;
        }
        if(flag_line == 1){
            grammar_line = grammar_line->next;
            while(grammar_line != NULL && grammar_line->g_no != index){
                grammar_line = grammar_line->next;
            }
            flag_line = 0;
        }
        
        if(grammar_line == NULL){
            printf("error in finding grammar rule in follow set");
            exit(EXIT_FAILURE);
        }
        
        first_node * first_line = NULL;
        //first_line->eps= true;
        while(grammar_line->next != NULL && grammar_line->next->g_no != OR_RULE){
            //printf(" boo : %s", grammar_line->next->name);
            grammar_line = grammar_line->next;
            //printf("calling first of : %s at line %d\n", grammar_line->name, line);
            first_line = first(grammar_line->g_no, grammar);
            first_node * last = head;
            
            if(head->index == -1 && head->next == NULL)
                head = first_line;
            else{
                while(last->next != NULL){
                    last = last->next;
                }
                
                
                last->next = first_line;
            }
            
            
            
            if(first_line->eps != true){
                break;
            }
        }
        // if(first_line == NULL)
        // 	printf("yo");
        // else
        // 	printf("no");
        if(first_line == NULL || first_line->eps == true){
            if(index == grammar[line].g_no){
                g_set_index = g_set_index->next;
                //printf("continuing %d %d", index, g_set_index->line);
                
                continue;
            }
            //printf("calling follow of : %s", grammar[line].name);
            first_node * follow_lhs = follow(grammar[line].g_no, grammar);
            first_node * last_1 = head;
            if(head->index == -1 && head->next == NULL)
                head = follow_lhs;
            else{
                while(last_1->next != NULL)
                    last_1 = last_1->next;
                last_1->next = follow_lhs;
            }
        }
        if(g_set_index->next != NULL && g_set_index->line == g_set_index->next->line)
            flag_line = 1;
        g_set_index = g_set_index->next;
        
        
    }
    if(index == RULE_INDEX){
        first_node * x = (first_node*)malloc(sizeof(first_node));
        first_node * y = (first_node*)malloc(sizeof(first_node));
        x->index = NON_TERM_INDEX1;
        y->index = NON_TERM_INDEX2;
        x->next = y;
        y->next = head;
        head = x;
    }

    first_node * verify = NULL;
    first_node * current;
    int flag_ver = 0;
    while(head != NULL){
        current = verify;
        if(head->index <0)
            head = head->next;
        while(current != NULL){
            if(head->index != current->index)
                current = current->next;
            else{
                
                flag_ver = 1;
                break;
            }
        }
        if(flag_ver == 0){
            first_node * new = (first_node *)malloc(sizeof(first_node));
            new->index = head->index;
            new->next = verify;
            verify =new;
        }
        head = head->next;
        
    }
    
    
    
    return verify;
    
}
////////////////////////////////////////// FOLLOW END //////////////////////////////////////////////////////////////////////




////////////////////////////////////////// CREATE PARSE TABLE //////////////////////////////////////////////////////////////////////


void createParseTable(struct table *T,g_node * grammar){
    int i = 0;
    
    
    while(i < MAX_RULES){
        short rule_number = 1;
        short or_flag = 1;
        
        int non_terminal_number;
        g_node * temp;
        temp = grammar[i].next;                 //First element on RHS of rule!
        non_terminal_number = grammar[i].g_no;
        
        while(or_flag == 1){
            or_flag = 0;
            char rule[10];
            
            char line_number_str[5];            //String of line number
            sprintf(line_number_str,"%d",i);
            strcpy(rule,line_number_str);
            strcat(rule,"-");
            
            char rule_number_str[5];            //String of rule number in grammar
            sprintf(rule_number_str,"%d",rule_number);
            strcat(rule,rule_number_str);
            rule_number++;
            
            bool follow_epsilon_flag = false;

            int index = temp->g_no;             //The index number of the element on RHS rule.
            if(index == 106){
                follow_epsilon_flag = true;
            }
            //printf("\nthe index is : %d and the string is : %s\n",index,temp->name);
            first_node * head;
            head = first(index,grammar);
            
            //printf("\n\n\n\neps is : %d\n",head->eps);
            bool epsilon_flag = true;
            int terminal_number;
                        int while_count = 0;
            while(epsilon_flag && temp != NULL && temp->g_no != OR_RULE){
                while_count++;
                //printf("While loop running %d\n",while_count);
                //printf("\nthe index is : %d and the string is : %s\n",index,temp->name);

                epsilon_flag = head->eps;
                //printf("1...\n\n\n\neps is : %d\n",head->eps);
                //printf("1.this is : %d \n",head->index);
                
                
                
                terminal_number = head->index;          //The index_number of terminal in first of grammar rule!
                
                
                strcpy(T->cell[non_terminal_number][terminal_number - NO_NON_TERM].rule,rule);   //Updating table
                //printf("the rule written at [%d][%d] is : %s\n",non_terminal_number,terminal_number-NO_NON_TERM,T->cell[non_terminal_number][terminal_number - NO_NON_TERM].rule);
                
                first_node * current;
                current = head->next;
                
                while(current != NULL){
                    //printf("In here ");
                    //printf("this is : %d\n",current->index);
                    
                    terminal_number = current->index;
                    strcpy(T->cell[non_terminal_number][terminal_number - NO_NON_TERM].rule,rule);
                    //printf("the rule written at [%d][%d] is : %s\n",non_terminal_number,terminal_number-NO_NON_TERM,T->cell[non_terminal_number][terminal_number - NO_NON_TERM].rule);
                    
                    current = current->next;
                }
                
                if(epsilon_flag){
                    //printf("In here");
                    temp = temp->next;
                    index = temp->g_no;
                    head = first(index,grammar);
                }
                
            }
            
//            if(while_count > 1){
//                printf("the while count is : %d\n",while_count);
//            }
            
            if(epsilon_flag || follow_epsilon_flag){
                //Write follow code
                
                //int terminal_number;
                
                //printf("\nI am inside this\n");
                first_node *follow_head;
                follow_head = follow(non_terminal_number,grammar);
                
                terminal_number = follow_head->index;          //The index_number of terminal in first of grammar rule!
                
                
                strcpy(T->cell[non_terminal_number][terminal_number - NO_NON_TERM].rule,rule);   //Updating table
                //printf("the rule written at [%d][%d] is : %s\n",non_terminal_number,terminal_number,T->cell[non_terminal_number][terminal_number - NO_NON_TERM].rule);
                
                first_node * follow_current;
                follow_current = follow_head->next;
                
                while(follow_current != NULL){
                    //printf("In here \n");
                    //printf("this is : %d\n",follow_current->index);
                    
                    terminal_number = follow_current->index;
                    strcpy(T->cell[non_terminal_number][terminal_number - NO_NON_TERM].rule,rule);
                    //printf("the rule written at [%d][%d] is : %s\n",non_terminal_number,terminal_number,T->cell[non_terminal_number][terminal_number-NO_NON_TERM].rule);
                    
                    follow_current = follow_current->next;
                }

            }
            
            
            while(temp && temp->g_no && temp->g_no != OR_RULE ){
                //strcat(rule,temp->name);
                temp = temp->next;
            }
            if(temp && temp->g_no == OR_RULE){
                //printf("temp is : %s\n",temp->name);
                temp = temp->next;
                //printf("Name after Or %d: %s\n",i,temp->name);
                or_flag = 1;
            }
            
            //            g_node * current;
            //            current = grammar[i].next;
            //            int index = current->g_no;
            //            printf("the index is : %d\n",index);
            //            first_node * head;
            //            head = first(index,grammar);
            //            printf("this is : %d\n",head->index);
            //
            //            first_node * temp;
            //            temp = head->next;
            //            while(temp != NULL){
            //                printf("In here ");
            //                printf("this is : %d\n",temp->index);
            //                temp = temp->next;
            //            }
            //            if(head->eps == true){
            //                //Write follow code
            //            }
            //printf("The rule is : %s\n",rule);
        }
        i++;
    }
    
}

 
////////////////////////////////////////// CREATE PARSE TABLE //////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////// PARSE_GRAMMAR ////////////////////////////////////////////////////////////////
//reads the file and creates the grammar structure for future use;




g_node * parse_grammar(char * filename, char * elements[]){
    FILE *fp;
    char *line = NULL;
    size_t bufsize = 100;
    size_t len;
    fp = fopen(filename, "r");
    if (fp == NULL){
        printf("error reading file");
        exit(EXIT_FAILURE);
    }



    //grammar data structure(array) contains grammar rules. linked list format ---- A -> B | C ----- in the form -> ----- A -> B -> | -> C ------ with each letter
    //represented as g_node.
    g_node * grammar = (g_node* )malloc(MAX_RULES*sizeof(g_node));
    int i = 0;
    while((len = getline(&line, &bufsize, fp)) != -1){
        
        char s[] = " <>-";
        char *p = strtok(line,s);
        strcpy(grammar[i].name, p);
        int flag = 0;
    //     printf("hey");
    // fflush(stdout);
        for(int j = 0; j<NO_NON_TERM; j++){
            int x = strcmp(p, elements[j]);
            if(x == 0){
                grammar[i].g_no = j;
                flag = 1;
                break;
            }
        }

        if(flag == 0){
            printf("error : token [%s] not found in token_list array.",p);
            exit(EXIT_FAILURE);
        }
        //printf("%s \n", grammar[i].name);
        grammar[i].type = non_term;
        g_node* current = &grammar[i];
        for (p = strtok(NULL, s); p != NULL; p = strtok(NULL, s))
        {
            
            current->next = (g_node*)malloc(sizeof(g_node));
            current = current->next;
            
            if(p[strlen(p)-1] == '\n')
                p[strlen(p)-1] = '\0';
            
            int x = isUpperCase(p);
            strcpy(current->name,p);
            printf("p is %s\n", p);
            if(x == 0){
                current->type = non_term;
                int flag = 0;
                for(int j = 0; j<NO_NON_TERM; j++){
                    int x = strcmp(p, elements[j]);
                    if(x == 0){
                        current->g_no = j;
                        flag = 1;
                        break;
                    }
                }
                if(current->g_no == NULL){
                    char eps[2] = "|";
                    if(strcmp(current->name, eps) == 0)
                        current->g_no = OR_RULE;
                }
            }else{
                current->type = term;
                int flag = 0;
                for(int j = NO_ELE-1; j>=NO_NON_TERM; j--){
                    int x = strcmp(p, elements[j]);
                    if(x == 0){
                        current->g_no = j;
                        flag = 1;
                        break;
                    }

                    
                }
                if(flag == 0){
                    printf("error: token not found in token_list array i.e. %s", p);
                    exit(EXIT_FAILURE);
                }
            }
            
            
        }
        i++;
        
        
    }
    fclose(fp);
    return grammar;
}

//////////////////////////////////////////////// PARSE_GRAMMAR END////////////////////////////////////////////////////////////////


////////////////////////////////////////////////MAIN////////////////////////////////////////////////////////////////
// int main(){
    
//     //elements array contains all nonterminals and then terminals.
//     char * elements[NO_ELE] = {"program","moduleDeclarations","moduleDeclaration","otherModules","driverModule","module","ret","input_plist","N1","output_plist","N2","dataType","type","moduleDef","statements","statement","ioStmt","var","whichId","simpleStmt","assignmentStmt","whichStmt","lvalueIDStmt","lvalueARRStmt","index","moduleReuseStmt","optional","idList","N3","expression","arithmeticOrBooleanExpr","N7","AnyTerm","N8","arithmeticExpr","N4","term","N5","factor","op1","op2","logicalOp","relationalOp","declareStmt","condionalStmt","caseStmts","N9","value","default","iterativeStmt","range",
// 								"RANGEOP", "WHILE", "IN", "FOR", "DEFAULT", "FALSE", "TRUE", "BREAK", "CASE", "SWITCH", "NE", "EQ", "GE", "GT", "LE", "LT", "OR", "AND", "DIV", "MUL", "PLUS", "MINUS", "PARAMETERS", "WITH", "USE", "ASSIGNOP", "RNUM", "NUM", "PRINT", "BC", "BO", "END", "START", "OF", "ARRAY", "BOOLEAN", "REAL", "INTEGER", "COMMA", "COLON", "RETURNS", "SQBC", "SQBO", "INPUT", "TAKES", "ENDDEF", "DEF", "DRIVERENDDEF", "PROGRAM", "DRIVER", "DRIVERDEF", "SEMICOL", "ID", "MODULE", "DECLARE", "EPSILON", "GET_VALUE", "|", "$"};
    

    
//     char * filename = "grammar.txt";
//     g_node * grammar = parse_grammar(filename, elements);
//     //printf("%s", grammar[0].name);
    
//     // first_node * x = follow(23, grammar);
//     //printf("%d",x->eps);
//     //printf("hey");
//     // printf("ans is :\n");
//     // while(x != NULL){
//     //     printf("%s \n", elements[x->index]);
//     //     x=x->next;
//     // }
//     //printf("hey");
    
//     //g_set_node * set_rhs = get_grammar_array(grammar);
    
    
//     //printf("%d", set_rhs[2].line);
    
    
    
    
//     struct table *T;
//     T = (struct table *)malloc(sizeof(struct table));
//     createParseTable(T,grammar);

//     // for(int i = 0 ; i < NO_NON_TERM;i++){
//     //     for (int j = 0; j < NO_TERM + 1; j++) {
//     //         if(strcmp(T->cell[i][j].rule,"") != 0)
//     //             printf("the rule written at [%d][%d] is : %s\n",i,j,T->cell[i][j].rule);
//     //     }
//     // }
    
// //    first_node * head;
// //    head = first(1,grammar);
// //    printf("\n\n\n\neps is : %d\n",head->eps);
    
//     return 0;
// }
