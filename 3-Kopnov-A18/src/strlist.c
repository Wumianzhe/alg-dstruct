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

int strcmp(const char * str1, const char * str2) {
	// until end of str1
	int i=0;
	for(;str1[i];i++) {
		// if str2 ends, str2[i] = 0 and positive value is returned
		if (str1[i]-str2[i] != 0) {
			return str1[i]-str2[i];
		}
	}
	if (str2[i]!= 0) {
		return 1;
	} else {
		return 0;
	}
}

char* strcpy(char *dest,const char *src) {
	int i=0;
	for(; src[i]; i++) {
		dest[i]= src[i];
	}
	//copy 0
	dest[i]=src[i];
	return dest;
}
