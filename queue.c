// Sugam Garg - 2014A7PS092P, Divish Dayal - 2014A7PS132P

#include<stdio.h>

#include<stdlib.h>

#include<string.h>
#include"queueDef.h"

struct queue *creatQueue(){
    
    struct queue *Q;
    
    Q = (struct queue *)malloc(sizeof(struct queue));
    
    Q->size = 0;
    
    Q->front = Q->end = NULL;
    
    return Q;
    
}

void Enqueue(struct queue *Q,struct node *temp){
    
    Q->size++;
    
    if(Q->end) {
        
        Q->end->next = temp;
        
    }
    
    Q->end = temp;
    
    if(Q->front == NULL){
        
        Q->front = Q->end;
        
    }
    
    return;
    
}


struct node * Dequeue(struct queue *Q){
    
    if(Q->size == 0){
        
        printf("Queue is empty");
        
        return NULL;
        
    }
    Q->size--;
    
    struct node * temp;
    
    temp = Q->front;
    
    Q->front = Q->front->next;
    
    //free(temp);
    
    return temp;
    
}


void printQ(struct queue *Q){
    struct node *temp;
    temp = Q->front;
    
    while(temp != NULL){
        
        printf("%d  %s  %s   %d\n",temp->line_no,temp->id,temp->value,temp->id_no);
        temp = temp->next;
        
    }
    
    printf("Queue size is : %d\n",Q->size);
    
}

//int main(){
//    
//    int i = 0;
//    
//    struct queue *Q;
//    
//    Q = creatQueue();
//    
//    while(i<10){
//        
//        struct node *temp;
//        
//        temp = (struct node *)malloc(sizeof(struct node));
//        
//        strcpy(temp->id,"abc");
//        
//        temp->value = i;
//        
//        temp->line_no = i*10;
//
//        temp->id_no = 1;

//        temp->next = NULL;
//        
//        Enqueue(Q,temp);
//        
//        i++;
//        
//    }
//    
//    printQ(Q);
//    
//    Dequeue(Q);
//    
//    printQ(Q);
//    
//}
