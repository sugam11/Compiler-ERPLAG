// Sugam Garg - 2014A7PS092P, Divish Dayal - 2014A7PS132P

#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include"hashDef.h"

static int count = 0;
struct firstDataItem *createHashTable(int size){
    struct firstDataItem * HT;
    HT = (struct firstDataItem *)malloc(size * sizeof(struct firstDataItem));
    int i;
    for(i = 0;i < size;i++){
        HT[i].flag = 0;
        HT[i].next = (struct dataItem*)malloc(sizeof(struct dataItem));
    }
    return HT;
}

int hashValue(char *str)
{
    long hash = 5381;
    int c;
    int i = 0;
    while (i < strlen(str)){
        char ch = str[i];
        c = ch;
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
        i++;
    }
    return (hash%16);
}

void insert(char *keyword,int keyword_number,struct firstDataItem *HT){
    int hashIndex = hashValue(keyword);
    //printf("has index is : %d %d  %s\n\n",hashIndex,keyword_number,keyword);
    if(HT[hashIndex].flag == 0){
        HT[hashIndex].number = keyword_number;
        strcpy(HT[hashIndex].keyword,keyword);
        HT[hashIndex].flag = 1;
        HT[hashIndex].next = NULL;
        //printf("%d %d  %s  %d\n",count,HT[hashIndex].number,HT[hashIndex].keyword,HT[hashIndex].flag);
    }
    else{
        struct dataItem *temp = (struct dataItem *)malloc(sizeof(struct dataItem));
        strcpy(temp->keyword,keyword);
        temp->number = keyword_number;
        temp->next = NULL;
        struct dataItem *head = HT[hashIndex].next;
        if(head != NULL){
            while (head->next != NULL){
                head = head->next;
            }
            head->next = temp;
            //printf("head not 1 if : %d %d  %s\n",count,HT[hashIndex].next->number,HT[hashIndex].next->keyword);
        }
        else{
            HT[hashIndex].next = temp;
            //printf("head not 1 else : %d %d  %s\n",count,HT[hashIndex].next->number,HT[hashIndex].next->keyword);
        }
    }
    count++;
    
}

int search(char *keyword,struct firstDataItem *HT){
    int hashIndex = hashValue(keyword);
    if(HT[hashIndex].flag == 0){
        return HT[hashIndex].number;
    }
    if(strcmp(HT[hashIndex].keyword,keyword) == 0){
        return HT[hashIndex].number;
    }
    if(strcmp(HT[hashIndex].keyword,keyword) != 0){
        struct dataItem *temp;
        temp = HT[hashIndex].next;
        while(temp != NULL && strcmp(temp->keyword,keyword) != 0){
            temp = temp->next;
        }
        if(temp != NULL){
            return temp->number;
        }
        else{
            return -1;
        }
    }
    
}

//int main(){
//    struct firstDataItem *ht = createHashTable(16);
//    FILE *fp = fopen("keywords.txt","r");
//    
//    while(1){
//        int number;
//        char str[13];
//        fscanf(fp,"%d %s",&number,str);
//        if (feof(fp))
//            break;
//        insert(str, number, ht);
//        
//    }
//    printf("%d",search("array",ht));
//}
//
