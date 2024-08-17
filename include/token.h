#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#ifndef TOKEN_H
#define TOKEN_H

enum TokenType
{
    TOKEN_IDENTIFIER,
    TOKEN_KEYWORD,
    TOKEN_NUMBER,
    TOKEN_STRING,
    TOKEN_OPERATOR,
    TOKEN_DELIMITER,
    TOKEN_EOF
};

struct Token
{
    enum TokenType type;
    char *value;
};

struct Token make_token(enum TokenType type, const char value[]);

#endif // TOKEN_H
