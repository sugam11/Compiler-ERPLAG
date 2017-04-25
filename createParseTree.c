#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<stdbool.h>
#include"lexer.h"
#include"parseTable.h"
#include"parseTree.h"

#define AT 110


tree_node * createTree(g_node *grammar,struct table *T,struct queue * Q,char *elements[],char * filename){
    
    //root has line number 0 and value root.
    tree_node * root = tree_node_initialise(0, 0, "root");
    strcpy(root->name, elements[0]);
    
    tree_node *top = NULL;
    push(&top,AT,"@");
    
    push_node(&top,root);

    
    
    //    push(&top,grammar[3].g_no,grammar[3].name);
    //    push(&top,grammar[4].g_no,grammar[4].name);
    //    push(&top,grammar[3].g_no,grammar[3].name);
    
    struct node* queue_node;
    queue_node = getNextToken(Q);
    
    int count_flag = 0;
    while(1){
        count_flag++;
        if(top->num < NO_NON_TERM){
            char grammar_rule [20];
            //printf("%d  %d %d  th value of no is%d\n",top->num,queue_node->id_no,queue_node->id_no - NO_NON_TERM,NO_NON_TERM);
            strcpy(grammar_rule,T->cell[top->num][queue_node->id_no - NO_NON_TERM].rule);
            //printf("%s\n",grammar_rule);
            if(strlen(grammar_rule) == 0){
                break;
            }
            char * str_tok = "-";
            char * token = strtok(grammar_rule, str_tok);
            int line_number = atoi(token);
            token = strtok(NULL, str_tok);
            int section_no = atoi(token);
            g_node * rule = grammar[line_number].next;
            int or_count = 1;
            while(rule != NULL){
                if(rule->g_no == OR_RULE)
                    or_count++;
                if(or_count == section_no)
                    break;
                rule = rule->next;
            }
            if(rule == NULL){
                printf("error occured : section not found");
                exit(EXIT_FAILURE);
            }
            if(rule->g_no == OR_RULE)
                rule = rule->next;
            int arr[50];
            for(int i = 0; i < 50; i++){
                arr[i]= -1;
            }
            int count = 0;
            while(rule != NULL && rule->g_no != OR_RULE){
                arr[count++] = rule->g_no;
                rule = rule->next;
            }
            count--;
            //tree_node * current_popped = top->node_tree;
            //            printf("%s", current_popped->name);
            //            fflush(stdout);
            tree_node * current_popped = pop(&top);
            int this_while_count = 0;
            tree_node *tree_stack_top = NULL;
            while(count >= 0){
                //printf("%d\n",arr[count]);
                if(arr[count] != 106 && arr[count] < 111 && arr[count] > -1){
                    tree_node * new = NULL;
                    new = tree_node_initialise(arr[count], queue_node->line_no, elements[arr[count]]);
                    
                    //strcpy(new->name, elements[arr[count]]);
                    //printf("%s    %d\n",new->name, count);
                    //new->line_number = elements[arr]
                    push_node(&top,new);
                    
                    push_node(&tree_stack_top,new);
                    //tree_node * new = tree_node_initialise(top->num);
                    //strcpy(new->name, elements[top->num]);
                    //tree_insert_child(current_popped, new);
                    //in_order_traversal(root);
                    count--;
                    this_while_count++;
                }
                else if(arr[count] == 106){       //EPSILON TO BE ADDED  TO TREE TOO
                    tree_node * new = tree_node_initialise(arr[count], queue_node->line_no, "EPSILON");
                    strcpy(new->name, elements[arr[count]]);
                    tree_insert_child(current_popped, new);
                    break;
                }
                
            }
            while (this_while_count > 0) {
                tree_node * tree_stack_popped = pop(&tree_stack_top);
                tree_insert_child(current_popped, tree_stack_popped);
                this_while_count--;
            }
        }
        //printf("Printing stack here\n");
        //printStack(top);
        if(queue_node->id_no == top->num){
            memset(top->value,'\0',sizeof(top->value));
            strcpy(top->value, queue_node->value);
            top->line_no = queue_node->line_no;
            pop(&top);
            queue_node = getNextToken(Q);
            //printf("\n\n\n");
            //printStack(top);
            
        }
        if(top->num == AT){
            printf("\n----------------- COMPILATION SUCCESSFUL -----------------\n");
            break;
        }
    }
    if(top->num != AT || count_flag > 1000){
        printf("\n----------------- COMPILATION UNSUCCESSFUL -----------------\n");
    }
    
    //printStack(top);
    FILE * fp = fopen(filename, "w");
    in_order_traversal(root,fp);
    fclose(fp);
    return root;
}

