#ifndef PARSE_H
#define PARSE_H

typedef struct TOKEN_LIST_ TOKEN_LIST;

TOKEN_LIST *Tokenize(const char *str);
char *GetToken(TOKEN_LIST *token_list, int i);
int GetTokenNum(TOKEN_LIST *token_list);
void FreeTokenList(TOKEN_LIST *token);
void DumpTokenList(TOKEN_LIST *token);

#endif /* PARSE_H */
