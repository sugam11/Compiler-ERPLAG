// Sugam Garg - 2014A7PS092P, Divish Dayal - 2014A7PS132P

#include<stdio.h>
#include<stdlib.h>
#include"parseTreeDef.h"
#include<ctype.h>
#include<string.h>



//////////////TREE NODE /////////////////////////////////////////////////////////////////////

// typedef struct tree_node{
//    int num;
//    char name[20];
//    struct tree_node * lchild;
//    struct tree_node * rchild;
//    struct tree_node * parent;
//    struct tree_node * next;
// }tree_node;



// //////////////STACK NODE /////////////////////////////////////////////////////////////////////
// typedef struct stack_node{
//    int num;
//    char name[20];
//    struct stack_node * next;
// } stack_node;




//////////////STACK Otree_nodePS /////////////////////////////////////////////////////////////////////

void push(tree_node ** top,int num,char * name){
    
          tree_node * temp = (struct tree_node*)malloc(sizeof(struct tree_node));
          temp->num = num;
          temp->next = NULL;
          strcpy(temp->name,name);
          
          if(!*top){
              *top = temp;
          }
          else{
              temp->next = *top;
              *top = temp;
          }
        }

void push_node(tree_node ** top,tree_node * new){
    
    if(!*top){
        //printf("here");
        *top = new;
        //temp->next = NULL;
    }
    else{
        new->next = *top;
        *top = new;
    }
}

          void printStack(struct tree_node *top){
              struct tree_node *temp;
              //temp = (struct stack_node *)malloc(sizeof(struct stack_node));
              temp = top;
              if(!top)
                  return;
              //printf("data on stack is : %d   %s\n",temp->num,temp->name);
              while(temp != NULL){
                  printf("data on stack is : %d   %s\n",temp->num,temp->name);
                  temp = temp->next;
                  
              }
          }
int isEmptyStack(struct tree_node *top){
    return top == NULL;
}

          tree_node * pop(struct tree_node **top){
              tree_node * temp;
              if(isEmptyStack(*top))
                  return NULL;
              temp = *top;
              *top = (*top)->next;
              return temp;
          }
          

          int Top(struct tree_node *top){
              if(isEmptyStack(top))
                  return -1;
              return top->next->num;
          }
          
          
          
          ////////////////TREE OPS /////////////////////////////////////////////////////////////////////

          void tree_insert_sibling(tree_node * x, tree_node * node){
              if(x->rchild == NULL){
                  x->rchild = node;
                  node->parent = x;
              }else{
                  tree_node * current = x;
                  current = current->rchild;
                  while(current->rchild != NULL){
                      current = current->rchild;
                  }
                  current->rchild = node;
                  node->parent = current;
              }
          }
          void tree_insert_child(tree_node * parent, tree_node * node){
              //printf("in tree insert");
            //printf("hey");
              node->lchild = NULL;
              node->rchild = NULL;
              
              tree_node * current;
              current = parent;
              if(current->lchild != NULL){
                  tree_insert_sibling(current->lchild, node);
              }else{
                  parent->lchild = node;
                  node->parent = parent;
              }
              //printf("completed : %s", node->name);
              //fflush(stdout);
              
          }
          
          
          
          
          void in_order_traversal(tree_node * node, FILE *fp){
              //printf("In inorder\n");
              //if node is null
              if(node == NULL){
                  return;
                  
              }
              
              //lchild handling
              if(node->lchild != NULL){
                  //printf("In here\n");
                  in_order_traversal(node->lchild,fp);
              }
              
              //PRINTING !!!
              fprintf(fp, "%d ", node->line_no);
              fputs(node->value,fp);
              fputs(" ",fp);
              fputs(node->name,fp);
              fputs("\n",fp);
              
              
              //rchild handling
              if(node->lchild != NULL){
                  if(node->lchild->rchild != NULL){
                      tree_node * current_rchild = node->lchild->rchild;
                      while(current_rchild != NULL){
                          
                          in_order_traversal(current_rchild,fp);
                          current_rchild = current_rchild->rchild;
                      }
                  }
              }
              
              
          }
          
          tree_node * tree_node_initialise(int x, int line_no, char * value){
              tree_node * node = (tree_node*)malloc(sizeof(tree_node));
              node->lchild = NULL;
              node->rchild = NULL;
              node->parent = NULL;
              node->num = x;
              node->line_no = line_no;
              strcpy(node->value, value);
              strcpy(node->name, value);
              return node;
              
          }

////////////////MAIN /////////////////////////////////////////////////////////////////////

// int main(){
//    //always initialize new stack with NULL -> else ERROR.

//    //struct stack_node * node = NULL;

//    // push(&node,10,"asdfsa");
//    // push(&node,11,"asdfsaa");
//    //pop(&node);
//    // printf("%s\n",node->name);
//    // if(node->next != NULL){
//    //     printf("%s\n",node->next->name);
//    //     node = node->next;
//    // }
//    // printStack(node);

//    tree_node * head = tree_node_initialise(1);
//    tree_node * new = tree_node_initialise(2);
//    tree_node * new0 = tree_node_initialise(3);
//    tree_node * new1 = tree_node_initialise(4);
//    strcpy(head->name, "divish");

//    strcpy(new->name, "sugam");

//    strcpy(new0->name, "sugam1");

//    strcpy(new1->name, "sugam2");
//    tree_insert_child(head, new);
//    tree_insert_child(new, new0);
//    tree_insert_sibling(new, new1);
//    in_order_traversal(head);
   
// }
