#include "string.h"

#include "parse.h"

#define TOKEN_NUM_MAX 0x10

struct TOKEN_LIST_ {
	char tokens[TOKEN_NUM_MAX][0x100];
	int token_num;
};

static TOKEN_LIST tl;
const TOKEN_LIST *Tokenize(const char *str) {
	memset(&tl, 0, sizeof(TOKEN_LIST));
	const char *p1 = str;
	const char *p2 = str;
	int i = 0;
	while(*p2 != '\0') {
		while(*p1 != '\0' && *p1 == ' ') p1++;
		p2 = p1;
		if(*p2 == '\0') {
			return &tl;
		}
		while(*p2 != '\0' && *p2 != ' ') p2++;
		int len = (uintptr_t)p2 - (uintptr_t)p1;
		strncpy(tl.tokens[i], p1, len);
		tl.tokens[i][len] = '\0';
		tl.token_num++;
		i++;
		p1 = p2;
		if(i == TOKEN_NUM_MAX) return &tl;
	}
	return &tl;
}

const char *GetToken(const TOKEN_LIST *token_list, int i) {
	if(i >= token_list->token_num) return NULL;
	return token_list->tokens[i];
}

int GetTokenNum(const TOKEN_LIST *token_list) {
	return token_list->token_num;
}
