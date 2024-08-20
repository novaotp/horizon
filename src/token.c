#include "../include/token.h"

Token make_token(TokenType type, const char *value)
{
    Token token;
    token.type = type;

    token.value = (char *)malloc(strlen(value) + 1);
    if (token.value != NULL)
    {
        strcpy(token.value, value);
    }

    return token;
}
