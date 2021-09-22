#include "strlist.h"
#include <stdio.h>
#include <stdlib.h>

node_t* strnodeCreate(char* str) {
	int l=0;
	for(;str[l];l++);
	char* data = malloc(l+1);
	if(data) {
		strcpy(data, str);
	} else {
		perror("Malloc failed");
		return 0;
	}
	return nodeCreate(data);
}

void sortstrList(list_t *list) {
	//pointer type conversion to avoid making wrapper function
	sortList(list, (int(*)(const void*,const void*))strcmp);
}
