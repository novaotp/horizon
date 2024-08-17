#include "../include/lexer.h"
#include "../include/token.h"

struct Token *lexer(char *chars, size_t length)
{
    // + 1 size to add EOF token
    struct Token *tokens = (struct Token *)malloc(sizeof(struct Token) * (length + 1));

    for (size_t i = 0; i < length; i++)
    {
        char value[2] = {chars[i], '\0'};
        tokens[i] = make_token(TOKEN_NUMBER, value);
    }

    tokens[length - 1] = make_token(TOKEN_EOF, "\0");

    return tokens;
}
