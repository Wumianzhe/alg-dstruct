#ifndef _STRLIST_H
#define _STRLIST_H 1

#include "list.h"
#include <string.h>

node_t* strnodeCreate(char* str);
void sortstrList(list_t* list);

#endif
