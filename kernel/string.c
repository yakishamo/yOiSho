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

void *memmove(void *buf1, const void *buf2, size_t n) {
	char *d = buf1;
	const char *s = buf2;
	if(d==s) return d;

	if(d<s)
		for(;n;n--) *d++ = *s++;
	else 
		while(n)n--, d[n]=s[n];
	return buf1;
}
/*
	for(int i = 0; i < n; i++) {
		((char*)buf1)[i] = ((char*)buf2)[i];
	}
	return buf1;
}
*/

int strcmp(const char *s1, const char *s2) {
	const char *p1 = s1;
	const char *p2 = s2;
	while(*p1 == '\0' || *p2 == '\0') {
		if(*p1 - *p2 != 0) return *p1 - *p2;
		p1++;
		p2++;
	}
	return *p1-*p2;
}

int strncmp(const char *s1, const char *s2, size_t n) {
	for(int i = 0; i < n; i++) {
		if(s1[i] - s2[i] != 0 || s2[i] == '\0') return s1[i] - s2[i];
	}
	return 0;
}

char *strncpy(char *s1, const char *s2, size_t n) {
	int i;
	for(i = 0; i < n; i++) {
		s1[i] = s2[i];
		if(s2[i] == '\0') {
			break;
		}
	}
	for(;i < n; i++) {
		s1[i] = '\0';
	}
	return s1;
}

char *strcpy(char *s1, const char *s2) {
	int i = 0;
	while(s2[i] != '\0') {
		s1[i] = s2[i];
		i++;
	}
	s1[i] = '\0';
	return s1;
}

char *strcat(char *s1, const char *s2) {
	int s1_len = strlen(s1);
	strcpy(s1+s1_len, s2);
	return s1;
}
