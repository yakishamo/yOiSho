#include "string.h"

#include "../common/types64.h"
#include "../common/def.h"

int strlen(char *str) {
	int i = 0;
	while(str[i] != '\0') i++;
	return i;
}

void *memset(void* s, int c, size_t n) {
	for(size_t i = 0; i < n; i++) ((char*)s)[i] = c;
	return s;
}
