#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<stdbool.h>
#include"symbolTableDef.h"
#include"lexer.h"


int search_ST(struct file_read * curr,struct symbolTable *ST,int scope,bool declare_flag,bool module_flag){
    bool variable_flag = false;
        int i = 0;
    //printf("HERE : %d\n",ST->row_num);
    while(i < ST->row_num){
        //printf("here : %s   %d\n", ST->row[i].lexeme,ST->row[i].scopeNesting);
        if(strcmp(ST->row[i].lexeme,curr->lexeme) == 0){
            if(scope == ST->row[i].scopeNesting){
                //printf("here : %s   %d\n", ST->row[i].lexeme,ST->row[i].scopeNesting);
                return 1;
            }
            else if(scope > ST->row[i].scopeNesting){
                variable_flag = true;
            }
        }
        i++;
    }
    if(!declare_flag && !module_flag && !variable_flag){
        return 2;
    }
    return -1;
    
}



char * symbol_type(char *symbol_name,int scope,struct symbolTable *ST){
    int i = 0;
        while(i < ST->row_num){
        //printf("here : %s   %d\n", ST->row[i].lexeme,ST->row[i].scopeNesting);
        if(strcmp(ST->row[i].lexeme,symbol_name) == 0){
            if(scope >= ST->row[i].scopeNesting){
                //printf("here : %s   %d\n", ST->row[i].lexeme,ST->row[i].scopeNesting);
                return ST->row[i].type;
            }
        }
        i++;
    }
    return NULL;
}

int moduleSearch(char *module_name,struct symbolTable *ST){
    struct symbolTable * temp = ST;
    while(temp != NULL){
        if(strcmp(module_name,temp->module_name) == 0){
            return 1;
        }
        temp = temp->next;
    }
    return 0;
}


void print_ST(struct symbolTable *ST){
    //printf("In here\n");
    struct symbolTable * temp = ST;
    while(temp != NULL){
        int i = 0;
        while(i < temp->row_num){
            printf("%d  %s  %s  %s  %d  %d  %d  %d  %d\n",temp->row[i].s_no,temp->row[i].lexeme,temp->row[i].type,temp->row[i].scope,temp->row[i].startScope,temp->row[i].endScope,temp->row[i].scopeNesting,temp->row[i].id_width,temp->row[i].offset );
            i++;
        }
        temp = temp->next;
    }
}

struct symbolTable * createSymbolTable(struct symbolTable *ST){
    FILE *fp = fopen("tokens.txt","r");
    
    printf("\n\n\n IN SYMBOL TABLE \\\\\n");
    
    int univ_scope = 0;
    bool driver_flag = false , module_flag = false,module_input_flag = false,declare_flag = false,use_module_flag = false;
    char current_module[20];
    int start_module_scope = 0;
    int end_module_scope = 0;
    int curr_module_scope = 0;
    int curr_scope_start = 0,curr_scope_end = 0;
    int s_no = 1;
    int numOfTables = 0;
    int offset = 0;
    struct symbolTable *curr_ST = NULL;
    struct file_read * prev = NULL;
    while(1){
        struct file_read * curr = (struct file_read *)malloc(sizeof(struct file_read));
        
        
        if(fscanf(fp,"%s %s %d %d",curr->token,curr->lexeme,&curr->line_no,&curr->token_no) == EOF){
            break;
        }
        
        //printf("%d  %s  %s  %d\n",curr->line_no,curr->lexeme,curr->token,curr->token_no);
        fflush(stdout);
        //Actual Fxn to come !!
        
        if(strcmp(curr->token,"DRIVER") == 0){
            univ_scope++;
            driver_flag = true;
            if((strcmp(prev->token,"DRIVERDEF") == 0)) {
                curr_module_scope = 0;
                offset = 0;

                
                //Creating New Symbol Table Entry for a specific Module
                fflush(fp);
                //unsigned long file_curr_pos = ftell(fp);
                char module_name[20],module_token[20];
                int line_no,token_no;
                fscanf(fp,"%s %s %d %d",module_token,module_name,&line_no,&token_no);
                //printf("here : %d  %s  %s  %d\n",line_no,module_name,module_token,token_no);
                fflush(stdout);
                struct symbolTable * new_ST = (struct symbolTable *)malloc(sizeof(struct symbolTable));
                new_ST->row_num = 0;
                numOfTables++;
                memset(current_module,'\0',sizeof(current_module));
                strcpy(new_ST->module_name,module_name);
                strcpy(current_module,module_name);
                new_ST->prev = NULL;
                new_ST->next = NULL;
                if(ST == NULL){
                    ST = new_ST;
                    curr_ST = ST;
                }
                else{
                    curr_ST->next = new_ST;
                    new_ST->prev = curr_ST;
                    curr_ST = curr_ST->next;
                }
                
                //
                unsigned long file_curr_pos_before_start = ftell(fp);
                char lexer_token[20],lexeme1[20];
                fscanf(fp,"%s %s %d %d",lexer_token,lexeme1,&line_no,&token_no);
                while(token_no != 83){
                    fscanf(fp,"%s %s %d %d",lexer_token,lexeme1,&line_no,&token_no);
                }
                start_module_scope = line_no;
                //printf("The start scope is : %d\n",start_module_scope);
                int start_count = 1;
                while(token_no != 82 || start_count > 0){
                    fscanf(fp,"%s %s %d %d",lexer_token,lexeme1,&line_no,&token_no);
                    if(token_no == 83){         //83    : START
                        start_count++;
                        //printf("here : %d  %d\n",start_count,line_no);
                    }
                    if(token_no == 82){         //82 : END
                        start_count--;
                        //printf("here : %d  %d\n",start_count,line_no);
                    }
                }
                end_module_scope = line_no;
                //printf("The end scope is : %d\n",line_no);
                fseek(fp,file_curr_pos_before_start,0);
                
            }

        }
        
        else if(strcmp(curr->token,"MODULE") == 0){
            //printf("%s\n",prev->token);
            fflush(stdout);
            if((strcmp(prev->token,"DEF") == 0)) {
                curr_module_scope = 0;
                offset = 0;

                
                //Creating New Symbol Table Entry for a specific Module
                fflush(fp);
                //unsigned long file_curr_pos = ftell(fp);
                char module_name[20],module_token[20];
                int line_no,token_no;
                fscanf(fp,"%s %s %d %d",module_token,module_name,&line_no,&token_no);
                //printf("here : %d  %s  %s  %d\n",line_no,module_name,module_token,token_no);
                fflush(stdout);
                int search_module = moduleSearch(module_name,ST);
                if(search_module == 0){
                    struct symbolTable * new_ST = (struct symbolTable *)malloc(sizeof(struct symbolTable));
                    new_ST->row_num = 0;
                    numOfTables++;
                    memset(current_module,'\0',sizeof(current_module));
                    strcpy(new_ST->module_name,module_name);
                    strcpy(current_module,module_name);
                    new_ST->prev = NULL;
                    new_ST->next = NULL;
                    if(ST == NULL){
                        ST = new_ST;
                        curr_ST = ST;
                    }
                    else{
                        curr_ST->next = new_ST;
                        new_ST->prev = curr_ST;
                        curr_ST = curr_ST->next;                }
                    
                    //
                    unsigned long file_curr_pos_before_start = ftell(fp);
                    char lexer_token[20],lexeme1[20];
                    fscanf(fp,"%s %s %d %d",lexer_token,lexeme1,&line_no,&token_no);
                    while(token_no != 83){
                        fscanf(fp,"%s %s %d %d",lexer_token,lexeme1,&line_no,&token_no);
                    }
                    start_module_scope = line_no;
                    //printf("The start scope is : %d\n",start_module_scope);
                    int start_count = 1;
                    while(token_no != 82 || start_count > 0){
                        fscanf(fp,"%s %s %d %d",lexer_token,lexeme1,&line_no,&token_no);
                        if(token_no == 83){         //83    : START
                            start_count++;
                            //printf("here : %d  %d\n",start_count,line_no);
                        }
                        if(token_no == 82){         //82 : END
                            start_count--;
                            //printf("here : %d  %d\n",start_count,line_no);
                        }
                    }
                    end_module_scope = line_no;
                    //printf("The end scope is : %d\n",line_no);
                    fseek(fp,file_curr_pos_before_start,0);
                }
                else if(search_module == 1){
                    printf("module overloading : %s at line number : %d\n",module_name,curr->line_no);
                }
            }
        }
        
        else if(strcmp(curr->token,"ID") == 0 && (module_input_flag || declare_flag) && (!use_module_flag)){
            
            //check whether ID is already present in symbol table or not !!
            
            int keyword = search_ST(curr,curr_ST,curr_module_scope,declare_flag,module_input_flag);
            //Check the type of ID
//                printf("%d  %s  %s  %d\n",curr->line_no,curr->lexeme,curr->token,curr->token_no);
//            fflush(stdout);
            if(keyword == -1 ){
                curr_ST->row[curr_ST->row_num].s_no = s_no;
                curr_ST->row[curr_ST->row_num].line_no = curr->line_no;
                s_no++;
                if(curr_module_scope > 1){
                    curr_ST->row[curr_ST->row_num].startScope = curr_scope_start;
                    curr_ST->row[curr_ST->row_num].endScope = curr_scope_end;

                }
                else{
                    curr_ST->row[curr_ST->row_num].startScope = start_module_scope;
                    curr_ST->row[curr_ST->row_num].endScope = end_module_scope;
                }
                if(module_input_flag){
                    curr_ST->row[curr_ST->row_num].scopeNesting = curr_module_scope + 1;
                }
                else{
                    curr_ST->row[curr_ST->row_num].scopeNesting = curr_module_scope;
                }
                strcpy(curr_ST->row[curr_ST->row_num].lexeme,curr->lexeme);
                strcpy(curr_ST->row[curr_ST->row_num].scope,current_module);
                
                char lexer_token1[20],lexeme2[20];
                int line_no1,token_no1;
                int curr_file_pos = ftell(fp);
                fscanf(fp,"%s %s %d %d",lexer_token1,lexeme2,&line_no1,&token_no1);
                while(token_no1 != 90){          //90 : colon
                    fscanf(fp,"%s %s %d %d",lexer_token1,lexeme2,&line_no1,&token_no1);
                }
                memset(lexeme2,'\0',sizeof(lexeme2));
                fscanf(fp,"%s %s %d %d",lexer_token1,lexeme2,&line_no1,&token_no1);
                
                if(token_no1 == 85){
                //array handler
                    char array_type[20];
                    strcpy(array_type,"array");
                    while(token_no1 != 51){          //51 : RANGEOP
                        fscanf(fp,"%s %s %d %d",lexer_token1,lexeme2,&line_no1,&token_no1);
                    }
                    memset(lexeme2,'\0',sizeof(lexeme2));
                    fscanf(fp,"%s %s %d %d",lexer_token1,lexeme2,&line_no1,&token_no1);
                    strcat(array_type,"(");
                    strcat(array_type,lexeme2);
                    int multiplier = atoi(lexeme2);
                    strcat(array_type,",");
                    
                    while(token_no1 != 84){          //84 : OF
                        fscanf(fp,"%s %s %d %d",lexer_token1,lexeme2,&line_no1,&token_no1);
                    }
                    memset(lexeme2,'\0',sizeof(lexeme2));
                    fscanf(fp,"%s %s %d %d",lexer_token1,lexeme2,&line_no1,&token_no1);
                    strcat(array_type,lexeme2);
                    
                    if(strcmp(lexeme2,"integer") == 0){
                        curr_ST->row[curr_ST->row_num].id_width = 2 * multiplier;
                    }
                    if(strcmp(lexeme2,"real") == 0){
                        curr_ST->row[curr_ST->row_num].id_width = 4 * multiplier;
                    }
                    if(strcmp(lexeme2,"boolean") == 0){
                        curr_ST->row[curr_ST->row_num].id_width = 1 * multiplier;
                    }
                    curr_ST->row[curr_ST->row_num].offset = offset;
                    offset = offset + curr_ST->row[curr_ST->row_num].id_width;
                    strcat(array_type,")");
                    //printf("Array type is : %s\n",array_type);
                    strcpy(curr_ST->row[curr_ST->row_num].type,array_type);
                    //printf("%d  %s  %s  %s  %d  %d  %d\n",curr_ST->row[curr_ST->row_num].s_no,curr_ST->row[curr_ST->row_num].lexeme,curr_ST->row[curr_ST->row_num].type,curr_ST->row[curr_ST->row_num].scope,curr_ST->row[curr_ST->row_num].startScope,curr_ST->row[curr_ST->row_num].endScope,curr_ST->row[curr_ST->row_num].scopeNesting );
                    fseek(fp,curr_file_pos,0);
                    curr_ST->row_num = curr_ST->row_num + 1;
                }
                else{
                    strcpy(curr_ST->row[curr_ST->row_num].type,lexeme2);
                    //Type
                    if(strcmp(lexeme2,"integer") == 0){
                        curr_ST->row[curr_ST->row_num].id_width = 2;
                    }
                    if(strcmp(lexeme2,"real") == 0){
                        curr_ST->row[curr_ST->row_num].id_width = 4;
                    }
                    if(strcmp(lexeme2,"boolean") == 0){
                        curr_ST->row[curr_ST->row_num].id_width = 1;
                    }
                    curr_ST->row[curr_ST->row_num].offset = offset;
                    offset = offset + curr_ST->row[curr_ST->row_num].id_width;
                    //printf("%d  %s  %s  %s  %d  %d  %d  %d\n",curr_ST->row[curr_ST->row_num].s_no,curr_ST->row[curr_ST->row_num].lexeme,curr_ST->row[curr_ST->row_num].type,curr_ST->row[curr_ST->row_num].scope,curr_ST->row[curr_ST->row_num].startScope,curr_ST->row[curr_ST->row_num].endScope,curr_ST->row[curr_ST->row_num].scopeNesting ,curr_ST->row[curr_ST->row_num].id_width);
                    fseek(fp,curr_file_pos,0);
                    curr_ST->row_num = curr_ST->row_num + 1;
                }
            }
            else if (keyword == 1){
                printf("Redeclaration of variable '%s' in same scope at line number : %d\n",curr->lexeme,curr->line_no);
            }
            
            
            //curr_ST->row_num = curr_ST->row_num + 1;
        }
        
        else if(strcmp(curr->token,"ID") == 0 && !(module_input_flag || declare_flag) && (module_flag || driver_flag) && (!use_module_flag)){
            
            int search_result = search_ST(curr,curr_ST,curr_module_scope,declare_flag,module_input_flag);
            if(search_result == 2){
                printf("Variable '%s' not declared at line number : %d\n",curr->lexeme,curr->line_no);
            }
            
        }
        
        else if(strcmp(curr->token,"SWITCH") == 0){
            unsigned long file_curr_pos = ftell(fp);
            char lexer_token[20],lexeme1[20];
            int line_no,token_no = 0;
            fscanf(fp,"%s %s %d %d",lexer_token,lexeme1,&line_no,&token_no);
            if(strcmp(lexer_token,"BO") == 0){
                memset(lexeme1,'\0',sizeof(lexeme1));
                memset(lexer_token,'\0',sizeof(lexer_token));
                fscanf(fp,"%s %s %d %d",lexer_token,lexeme1,&line_no,&token_no);
                //printf("HERE : %s %s %d %d\n",lexer_token,lexeme1,line_no,token_no);

                char * type = symbol_type(lexeme1,curr_module_scope,ST);
                
                if(strcmp(type,"boolean") == 0){
                    memset(lexeme1,'\0',sizeof(lexeme1));
                    memset(lexer_token,'\0',sizeof(lexer_token));
                    fscanf(fp,"%s %s %d %d",lexer_token,lexeme1,&line_no,&token_no);
                    while (1) {
                        while(strcmp(lexeme1,"case") != 0 && strcmp(lexeme1,"end") != 0){
                            memset(lexeme1,'\0',sizeof(lexeme1));
                            memset(lexer_token,'\0',sizeof(lexer_token));
                            fscanf(fp,"%s %s %d %d",lexer_token,lexeme1,&line_no,&token_no);
                        }
                        memset(lexeme1,'\0',sizeof(lexeme1));
                        memset(lexer_token,'\0',sizeof(lexer_token));
                        fscanf(fp,"%s %s %d %d",lexer_token,lexeme1,&line_no,&token_no);
                        printf("HERE : %s\n",lexeme1);
                        if(strcmp(lexeme1,"true") != 0 && strcmp(lexeme1,"false") != 0 && strcmp(lexeme1,"end") != 0){
                            printf("A switch statement with a boolean type identifier can have the case statements with labels true and false only.Error at line no : %d\n",line_no);
                        }
                        if(strcmp(lexeme1,"end") == 0){
                            break;
                        }
                    }
                    
                }
                if(strcmp(type,"real") == 0){
                    printf("A switch statement with an identifier of type real is not valid. Error at line_no : %d\n",line_no);
                }
                if(strcmp(type,"integer") == 0){
                    
                }
                fseek(fp,file_curr_pos,0);
                //printf("The type is : %s\n",type);
            }
            else{
                printf("Syntax error at %d",curr->line_no);
            }
        }
        else if(strcmp(curr->token,"START") == 0){
            curr_module_scope++;
            curr_scope_start = curr->line_no;
            unsigned long file_curr_pos = ftell(fp);
            char lexer_token[20],lexeme1[20];
            int line_no,token_no = 0;
            int start_count = 1;
            while(token_no != 82 || start_count > 0){
                fscanf(fp,"%s %s %d %d",lexer_token,lexeme1,&line_no,&token_no);
                if(token_no == 83){         //83    : START
                    start_count++;
                    //printf("here : %d  %d\n",start_count,line_no);
                }
                if(token_no == 82){         //82 : END
                    start_count--;
                    //printf("here : %d  %d\n",start_count,line_no);
                }
            }
            curr_scope_end = line_no;
            //printf("The end scope is : %d\n",line_no);
            fseek(fp,file_curr_pos,0);
        }
        else if(strcmp(curr->token,"END") == 0){
            curr_module_scope--;
        }
        else if(strcmp(curr->token,"INPUT") == 0){
            module_input_flag = true;
        }
        else if(strcmp(curr->token,"RETURNS") == 0){
            module_input_flag = true;
        }
        else if(strcmp(curr->token,"DEF") == 0){
            module_flag = true;
            
            
        }
        
        else if(strcmp(curr->token,"DECLARE") == 0){
            declare_flag = true;
            char lexer_token1[20],lexeme2[20];
            int line_no1,token_no1;
            int curr_file_pos = ftell(fp);
            fscanf(fp,"%s %s %d %d",lexer_token1,lexeme2,&line_no1,&token_no1);
            if(strcmp(lexer_token1,"MODULE") == 0){
                declare_flag = false;
            }
            fseek(fp,curr_file_pos,0);
        }
        
        else if(strcmp(curr->token,"SEMICOL") == 0 && module_input_flag){
            module_input_flag = false;
        }
        else if(strcmp(curr->token,"SEMICOL") == 0 && declare_flag){
            declare_flag = false;
        }
        
        else if(strcmp(curr->token,"USE") == 0){
            use_module_flag = true;
        }
        else if(strcmp(curr->token,"SEMICOL") == 0 && use_module_flag){
            use_module_flag = false;
        }
        if(curr->line_no == end_module_scope){
            module_flag = false;
            if(driver_flag){
                driver_flag = false;
            }
        }
        prev = (struct file_read *)malloc(sizeof(struct file_read));
        prev->line_no = curr->line_no;
        prev->token_no = curr->token_no;
        strcpy(prev->lexeme,curr->lexeme);
        strcpy(prev->token,curr->token);
    }
    //printf("NO of tables are : %d\n",numOfTables);
    return ST;
}


//int main(){
//    struct symbolTable *ST = NULL;
//    ST = createSymbolTable(ST);
////    char * str = symbol_type("t",10,ST);
////    printf("%s\n",str);
//}
