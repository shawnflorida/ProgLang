#include "include/token.h"
#include <stdlib.h>

// int is the default parameter if it is an enum from token.h
token_t *init_token(int type, char *value, int l, int lc)
{

    // allocate a memory for the token
    token_t *token = calloc(1, sizeof(struct TOKEN_STRUCT));
    // whatever the type of the token, assign it as its own
    token->type = type;
    // whatever the character value, assign it as the token value
    token->value = value;
    token->lpos = l;
    token->cpos = lc;

    return token;
}