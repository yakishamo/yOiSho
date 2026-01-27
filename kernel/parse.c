#include "string.h"
#include "kmalloc.h"
#include "parse.h"

#define TOKEN_NUM_MAX 0x10

struct TOKEN_LIST_ {
  TOKEN_LIST *next;
  char str[];
};

TOKEN_LIST *Tokenize(const char *str) {
  TOKEN_LIST *token, *token_root;
  token_root = kmalloc(sizeof(TOKEN_LIST));
  token_root->next = NULL;
  token_root->str[0] = '\0';
  token = token_root;

	const char *p1 = str;
	const char *p2 = str;
	while(*p2 != '\0') {
    // skip space
		while(*p1 != '\0' && *p1 == ' ') p1++;
		p2 = p1;
		if(*p2 == '\0') {
			return token_root;
		}
		while(*p2 != '\0' && *p2 != ' ') p2++;
		int len = (uintptr_t)p2 - (uintptr_t)p1;
    token->next = kmalloc(sizeof(TOKEN_LIST) + len + 1);
    token->next->next = NULL;
		strncpy(token->next->str, p1, len);
		token->next->str[len] = '\0';
    token = token->next;
		p1 = p2;
	}
	return token_root;
}

char *GetToken(TOKEN_LIST *token, int i) {
  for(int j = -1; j < i; j++) {
    if(token == NULL) return NULL;
    token = token->next;
  }
  if(token == NULL) return NULL;
  return token->str;
}

int GetTokenNum(TOKEN_LIST *token) {
	int num = 0;
  while(token->next != NULL) {
    token = token->next;
    num++;
  }
  return num;
}

void FreeTokenList(TOKEN_LIST *token) {
  TOKEN_LIST *next = NULL;
  while(token != NULL) {
    next = token->next;
    kfree(token);
    token = next;
  }
}

void DumpTokenList(TOKEN_LIST *token) {
  if(token == NULL) return;
  token = token->next;
  while(token != NULL) {
    token = token->next;
  }
}
