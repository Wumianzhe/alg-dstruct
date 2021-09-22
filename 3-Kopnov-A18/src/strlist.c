#include "strlist.h"
#include <string.h>

void sortstrList(list_t *list) {
	//pointer type conversion to avoid making wrapper function
	sortList(list, (int(*)(const void*,const void*))strcmp);
}
