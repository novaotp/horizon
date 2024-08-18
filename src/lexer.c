#include "../include/lexer.h"
#include "../include/token.h"

struct Token *lexer(char *chars, size_t length)
{
    // + 1 size to add EOF token
    struct Token *tokens = (struct Token *)malloc(sizeof(struct Token) * (length + 1));

    size_t j = 0;
    enum TokenType type;
    for (size_t i = 0; i < length; i++)
    {
        if (chars[i] == '\r' || chars[i] == '\n' || chars[i] == '\0')
        {
            continue;
        }

        char value[2] = {chars[i], '\0'};

        if (isdigit(chars[i]))
        {
            type = TOKEN_INT;
        }
        else
        {
            type = TOKEN_STRING;
        }

        tokens[j++] = make_token(type, value);
    }

    tokens[j++] = make_token(TOKEN_EOF, "\0");

    // Resize tokens to account for skipped characters.
    tokens = realloc(tokens, sizeof(struct Token) * j);

    return tokens;
}
