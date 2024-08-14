#ifndef STRING_H
#define STRING_H

#include "../common/types64.h"
#include "../common/def.h"

int strlen(char *str);
void *memset(void *s, int c, size_t n);
void *memmove(void *buf1, const void *buf2, size_t n);
int strcmp(const char *s1, const char *s2);
int strncmp(const char *s1, const char *s2, size_t n);
char *strncpy(char *s1, const char *s2, size_t n);
char *strcpy(char *s1, const char *s2);
char *strcat(char *s1, const char *s2);

#endif /* STRING_H */
