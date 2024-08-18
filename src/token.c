#include "../include/token.h"

struct Token make_token(enum TokenType type, const char *value)
{
    struct Token token;
    token.type = type;

    token.value = (char *)malloc(strlen(value) + 1);
    if (token.value != NULL)
    {
        strcpy(token.value, value);
    }

    return token;
}
