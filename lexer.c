// Sugam Garg - 2014A7PS092P, Divish Dayal - 2014A7PS132P

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"hashtable.h"
#include"queue.h"
#include<ctype.h>



static int line_number = 0;

void populateHashTable(struct firstDataItem *ht){
    FILE *fp = fopen("keywords.txt","r");
    while(1){
        int number;
        char str[13];
        fscanf(fp,"%d %s",&number,str);
        //printf("%d %s\n",number,str);
        insert(str, number, ht);
        if (feof(fp))
            break;
    }
}


struct node * getNextToken(struct queue *Q){
    if(Q->size < 1){
        struct node *temp;
        temp = (struct node *)malloc(sizeof(struct node));
        strcpy(temp->id,"$");
        strcpy(temp->value,"$");
        temp->line_no = -1;
        temp->id_no = 109;
        temp->next = NULL;
        return temp;
    }
    struct node * temp = Dequeue(Q);
    return temp;
}
//int main(int argc,char ** argv){
//    struct queue * Q;
//    Q = creatQueue();
//    removeComments(argv[1],"cleanfile.txt");
//    fp = fopen("tokens.txt","w");
//    struct firstDataItem *ht = createHashTable(16);
//    populateHashTable(ht);
//    generateTokens(Q,ht);
//    printQ(Q);
//}

void addToQueue(struct queue *Q,char * keyword,char *lexeme,int keyword_number,FILE *token_fp){
    struct node *temp;
    
    temp = (struct node *)malloc(sizeof(struct node));
    
    strcpy(temp->id,keyword);
    
    strcpy(temp->value,lexeme);
    
    temp->line_no = line_number;
    
    temp->id_no = keyword_number;
    
    temp->next = NULL;
    
//    FILE *fp = fopen("Queue_tokens.txt","a");
    fputs(temp->id,token_fp);
    fputs("  ",token_fp);
    fputs(temp->value,token_fp);
    fputs("  ",token_fp);
    fprintf(token_fp,"%d",temp->line_no);
    fputs("  ",token_fp);
    fprintf(token_fp,"%d",temp->id_no);
    fputs("\n",token_fp);
    
    fflush(token_fp);
    Enqueue(Q,temp);
    
}


void matcher(char *buf,struct firstDataItem *ht,struct queue *Q,FILE *token_fp){
    int i = 0;
    int length = strlen(buf);
    char ch = buf[i];
    
    while(i < length){
        //printf("lookahead is : %c\n",ch);
        if(ch == '+'){
            
            addToQueue(Q,"PLUS","+",71,token_fp);
            
        }
        else if(ch == '-'){
            
            addToQueue(Q,"MINUS","-",72,token_fp);
        }
        else if(ch == '/'){
            
            addToQueue(Q,"DIV","/",69,token_fp);
        }
        else if(ch == '*'){
            
            addToQueue(Q,"MUL","*",70,token_fp);
        }
        else if(ch == '<'){
            if(buf[i+1] != '\0' && buf[i+1] == '='){
                
                addToQueue(Q,"LE","<=",65,token_fp);
                i++;
            }
            else if(buf[i+1] != '\0' && buf[i+1] == '<'){
                if(buf[i+2] != '\0' && buf[i+2] == '<'){
                    
                    addToQueue(Q,"DRIVERDEF","<<<",101,token_fp);
                    i = i + 2;
                }
                else{

                    addToQueue(Q,"DEF","<<",97,token_fp);
                    i++;
                }
            }
            else{

                addToQueue(Q,"LT","<",66,token_fp);
            }
        }
        
        else if(ch == '>'){
            if(buf[i+1] != '\0' && buf[i+1] == '='){

                addToQueue(Q,"GE",">=",63,token_fp);
                i++;
            }
            else if(buf[i+1] != '\0' && buf[i+1] == '>'){
                if(buf[i+2] != '\0' && buf[i+2] == '>'){

                    addToQueue(Q,"DRIVERENDDEF",">>>",98,token_fp);
                    i = i + 2;
                }
                else{

                    addToQueue(Q,"ENDDEF",">>",96,token_fp);
                    i++;
                }
            }
            else{

                addToQueue(Q,"GT",">",64,token_fp);
            }

        }
        else if(ch == '='){
            if(buf[i+1] != '\0' && buf[i+1] == '='){

                addToQueue(Q,"EQ","==",62,token_fp);
                
            }
            else{
                printf("\n Unknown pattern %c%c",buf[i],buf[i+1]);

            }
            i++;
        }
        else if(ch == '!'){
            if(buf[i+1] != '\0' && buf[i+1] == '='){

                addToQueue(Q,"NE","!=",61,token_fp);
            }
            else{
                printf("\n Unknown pattern %c%c",buf[i],buf[i+1]);

            }
            i++;
        }
        else if(ch == ':'){
            if(buf[i+1] != '\0' && buf[i+1] == '='){

                addToQueue(Q,"ASSIGNOP",":=",76,token_fp);
                i++;
            }
            else{

                addToQueue(Q,"COLON",":",90,token_fp);
            }
        }
        else if(ch == '.'){
            if(buf[i+1] != '\0' && buf[i+1] == '.'){

                addToQueue(Q,"RANGEOP","..",51,token_fp);
            }
            else{
                printf("\n Unknown pattern %c%c",buf[i],buf[i+1]);

            }
            i++;
        }
        else if(ch == ';'){

            addToQueue(Q,"SEMICOL",";",102,token_fp);
        }
        else if(ch == ','){

            addToQueue(Q,"COMMA",",",89,token_fp);
        }
        else if(ch == '['){

            addToQueue(Q,"SQBO","[",93,token_fp);
        }
        else if(ch == ']'){

            addToQueue(Q,"SQBC","]",92,token_fp);
        }
        else if(ch == '('){

            addToQueue(Q,"BO","(",81,token_fp);
        }
        else if(ch == ')'){

            addToQueue(Q,"BC",")",80,token_fp);
        }
        else if(  (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') ){
            
            char string[20] = "";
            int j = 0;
            
            string[j++] = ch;
            i++;
            while( ( ( buf[i] >= 'a' && buf[i] <= 'z') || ( buf[i] >= 'A' && buf[i] <= 'Z') || (buf[i] >= '0' && buf[i] <= '9') || (buf[i] == '_') ) && buf[i] != '\n') {
                string[j++] = buf[i++];
            }
            i--;
            int keyword_number = search(string,ht);
            if(keyword_number >= 0){

                int k;
                char upper_string[20] = "";
                for(k = 0; k < strlen(string); k++){

                    upper_string[k] = toupper(string[k]);
                }
                addToQueue(Q,upper_string,string,keyword_number,token_fp);
            }
            else{
                if(strlen(string) <= 8){

                    addToQueue(Q,"ID",string,103,token_fp);
                }
                else{
                    printf("Identifier at line number %d is longer than prescribed length",line_number);
                }
            }
            
        }
        else if(ch >= '0' && ch <= '9'){
            char num_string[20] = "";
            int h = 0;
            
            num_string[h++] = ch;
            i++;
            while (buf[i] >= '0' && buf[i] <= '9' && buf[i] != '\0') {
                num_string[h++] = buf[i++];
            }
            if(buf[i++] == '.'){
                if(buf[i] == '.'){
                    i = i - 2;

                    addToQueue(Q,"NUM",num_string,78,token_fp);
                    //printf("%d this is %c\n",line_number,buf[i]);
                }
                else if(buf[i] >= '0' && buf[i] <= '9'){
                    while (buf[i] >= '0' && buf[i] <= '9' && buf[i] != '\0') {
                        num_string[h++] = buf[i++];
                    }
                    if(buf[i] == 'E' || buf[i] == 'e'){
                        num_string[h++] = buf[i++];
                        if(buf[i] == '+' || buf[i] == '-' || (buf[i] >= '0' && buf[i] <= '9')){
                            num_string[h++] = buf[i++];
                            while (buf[i] >= '0' && buf[i] <= '9' && buf[i] != '\0') {
                                num_string[h++] = buf[i++];
                            }

                            addToQueue(Q,"RNUM",num_string,77,token_fp);
                            i--;
                            //printf("%d this is %c\n",line_number,buf[i]);
                        }
                    }
                    else{
                        i--;

                        addToQueue(Q,"RNUM",num_string,77,token_fp);
                        //printf("%d this is %c\n",line_number,buf[i]);
                    }
                }
            }
            else if(buf[i] == 'E' || buf[i] == 'e'){
                num_string[h++] = buf[i++];
                if(buf[i] == '+' || buf[i] == '-' || (buf[i] >= '0' && buf[i] <= '9')){
                    num_string[h++] = buf[i++];
                    while (buf[i] >= '0' && buf[i] <= '9' && buf[i] != '\0') {
                        num_string[h++] = buf[i++];
                    }

                    addToQueue(Q,"RNUM",num_string,77,token_fp);
                    i--;
                    //printf("%d this is %c\n",line_number,buf[i]);
                }
            }
            else{

                addToQueue(Q,"NUM",num_string,78,token_fp);
                i = i - 2;
                //printf("%d this is %c\n",line_number,buf[i]);
            }
        }
        else if(buf[i] == '\n'){

        }
        else if(buf[i] != ' ' && buf[i] != '\t' && buf[i] != '\r' && buf[i] != '\0'){
            printf("\nUnknown symbol '%c' at line %d, column %d",buf[i],line_number, i+1);

        }
        i++;
        while (buf[i] == ' ' || buf[i] == '\t') {
            i++;
        }
        ch = buf[i];
    }
}


char *getStream(FILE * gsfp){              //Get line from input file into memory
    char *str = (char *) malloc(100 * sizeof(char));
    fgets(str,100,gsfp);
    if(feof(gsfp))
        return str;
    if (strlen(str) == 100){
        int index = 99;
        while(!(str[index] == ' ' || str[index] == '\t')){
            index--;
        }
        str[index+1] = '\0';
        fseek(gsfp,-(99-index),SEEK_CUR);
    }	
    return str;
}

void generateTokens(struct queue *Q,struct firstDataItem *ht,FILE *token_fp){
    FILE * cf;
    cf = fopen("cleanfile.txt","r");
    while(1){
        char str[100];
        strcpy(str,getStream(cf));
        line_number++;
        //printf("%s",str);
        if(strcmp(str,"") == 0){
            break;
        }
        else{
            matcher(str,ht,Q,token_fp);
        }
    }
}

void removeComments(char *testcaseFile,char * cleanFile){
    short flag = 0;
    FILE * f1;
    FILE * main_file = fopen(testcaseFile,"r");
    f1 = fopen(cleanFile,"w");
    while (1) {
        char buf[100] = "";
       strcpy(buf,getStream(main_file));
       //printf("the string is : %s\n",buf);
       if(strcmp(buf,"") != 0){
           int i = 0;
           int length = strlen(buf);
           while(i < length){
               //printf("Flag is : %d\n",flag);
               if(flag == 1){
                   while(i < length && !((buf[i]=='*')&&(buf[i+1]=='*'))){
                       i++;
                   }
               }
               //printf("While cond : %d  %d   %c   %c",i,length,buf[i],buf[i+1]);
               while(i < length && !((buf[i]=='*')&&(buf[i+1]=='*'))){
                   fprintf(f1,"%c",buf[i]);
                   i++;
               }
               //printf("This is : %c%c\n",buf[i],buf[i+1]);
               if(i != length){
                   i = i + 2;
                   if(flag == 1){
                       flag = 0;
                       continue;
                   }
                   else{
                       flag = 1;
                       continue;
                   }
                   
               }
           }
       }
       else{
           break;
       }
    
    }
    fclose(f1);
}
