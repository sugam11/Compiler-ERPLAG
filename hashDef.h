// Sugam Garg - 2014A7PS092P, Divish Dayal - 2014A7PS132P

#ifndef _hashDef
#define _hashDef

struct firstDataItem{
    int number;
    char keyword[13];
    short flag;
    struct dataItem *next;
};

struct dataItem{
    int number;
    char keyword[13];
    struct dataItem *next;
};
#endif
