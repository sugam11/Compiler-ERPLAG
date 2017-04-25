#include<stdio.h>
#include<stdlib.h>
#include"parseTreeDef.h"
#include<ctype.h>
#include<string.h>

       /////////////////// ///////////////// --- TREE OPS --- ///////////////////////////////////////////////////
            
            //deleting a node and rejoining pointers
            void ast_node_delete(tree_node *n){
              //printf(" parent %d", n->parent->num);
              //fflush(stdout);
              if(n->parent && n->parent->rchild && n == n->parent->rchild){
                  if(n->rchild){
                    n->rchild->parent = n->parent;
                    n->parent->rchild = n->rchild;
                  }
                  else{
                    n->parent->rchild = NULL;
                  }
              }
              else if(n->parent && n->parent->lchild && n == n->parent->lchild){
                // printf(" parent %d", n->parent->num);
                // fflush(stdout);
                if(n->rchild){
                  n->parent->lchild = n->rchild;
                  n->rchild->parent = n->parent;
                }else
                  n->parent->lchild = NULL;
              }
              else if(!n->parent)
                printf("no parent of - %d\n", n->num);
              
            }

            //to remove all the nodes in the parse tree that are useless in ast.
            void remove_scrap(tree_node * root){
              // printf("new level %s\n", root->value);
              // fflush(stdout);
              tree_node * current = root;

              while(current != NULL){
                // printf(" doing1-%s\n", current->name);
                // fflush(stdout);
                
              
                if(current->num == 106 || //EPSILON
                   current->num == 102 || //SEMICOL
                   current->num == 90 || //COLON
                   current->num == 89 || //COMMA
                   current->num == 92 || //SQBC
                   current->num == 93 || //SQBO
                   current->num == 51 || //RANGEOP
                   current->num == 76 || //ASSIGNOP
                   current->num == 81 || //BO
                   current->num == 80 || //BC
                   current->num == 95 || //TAKES
                   current->num == 94 || //INPUT
                   current->num == 84 || //OF
                   current->num == 83 || //START
                   current->num == 82 || //END
                   current->num == 74 || //WITH
                   current->num == 73 || //PARAMETERS
                   current->num == 53 //IN
                  )
                {
                  ast_node_delete(current);
                  // printf("deleted - %d\n", current->num);
                  //current->visit = 2;
                  // fflush(stdout);
                }
                else if(current->lchild)
                  remove_scrap(current->lchild);
                
                // if(current->rchild)
                //   current->rchild->parent = current;
                //current->visit = 1;
                current=current->rchild;

              }
              // printf("\nexiting a level\n");
              // fflush(stdout);
            }

            int is_op(tree_node * node){
              if(node->num >= 61 && node->num <= 72)
                return 1;
              else
                return 0;
            }



            //make ast out of the parse tree
            void up(tree_node * root){
              //while(root){
                

                //if root is an operator that needs to be shifted up.
                if(root->parent 
                  //&& is_op(root)
                  ){  if(root == root->parent->lchild){
                          tree_node * temp = root->parent;
                          if(root == temp->lchild){ //root is a lchild of its parent

                            //assuming that this node does not has any lchild.
                            root->parent = temp->parent;
                            if(temp->parent->rchild == temp)
                              temp->parent->rchild = root;
                            else 
                              temp->parent->lchild = root;

                            if(temp->rchild){
                              root->rchild = temp->rchild;
                              temp->rchild->parent = root;
                            }
                            
                          }
                      }else{ //root is a rchild of its parent
                            if(root->rchild){
                              root->parent->rchild = root->rchild;
                              root->rchild->parent = root->parent;
                             }
                            else
                              root->parent->rchild = NULL;

                            tree_node * current = root;
                            while(current->parent->rchild == current)
                              current = current->parent;
                            tree_node * temp = current->parent;

                            root->rchild = temp->rchild;
                            temp->rchild->parent = root;

                            root->parent = temp->parent;
                            if(temp->parent->rchild == temp)
                              temp->parent->rchild = root;
                            else
                              temp->parent->lchild = root;

                            root->lchild = current;
                            current->parent = root;
                          }

                }

              }


              int make_ast(tree_node * root, int i){
                int count = 0;
                tree_node * x = root;
                int flag = 0;
                while(x){
                  if(x->lchild)
                    while(x->lchild && !x->lchild->rchild){
                      up(x->lchild);
                      flag = 1;
                      x = x->lchild;
                      count++;
                    }
                    if(!flag)
                      make_ast(x->lchild, 0);
                    
                    // if(x->num < 52)
                    //   if(!x->lchild)
                    //     ast_node_delete(x);

                    x = x->rchild;
                }
                return count;

              }
            

            void handle_no_child(tree_node * root){
              tree_node * x = root;
              
              while(x){
                
                if(!x->lchild)
                    if(x->num < 52 )
                      ast_node_delete(x);
                else
                  handle_no_child(x->lchild);
                x=x->rchild;
              }

            }




