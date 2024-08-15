#ifndef PARSE_H
#define PARSE_H

typedef struct TOKEN_LIST_ TOKEN_LIST;

const TOKEN_LIST *Tokenize(const char *str);
const char *GetToken(const TOKEN_LIST *token_list, int i);
int GetTokenNum(const TOKEN_LIST *token_list);

#endif /* PARSE_H */
