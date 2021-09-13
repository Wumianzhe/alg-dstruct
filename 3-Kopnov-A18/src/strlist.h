#ifndef _STRLIST_H
#define _STRLIST_H 1

#include "list.h"

node_t* strnodeCreate(char* str);
void sortstrList(list_t* list);

#ifndef _STRING_H
int strcmp(const char * str1, const char * str2);
char* strcpy(char *dest,const char *src);
#endif

#endif
