#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include "../include/lexer.h"
#include "../include/token.h"

struct Token make_alloc_failure_token()
{
    return (struct Token){TOKEN_EOF, NULL};
}

// Function to handle numbers and create a token for them
struct Token make_number_token(char *chars, size_t *i, size_t length)
{
    /// Initial buffer capacity
    size_t buffer_capacity = 16;
    char *buffer = (char *)malloc(buffer_capacity);
    if (buffer == NULL)
    {
        return make_alloc_failure_token();
    }

    size_t buffer_index = 0;
    bool is_dot_encountered = false;

    while (*i < length && (isdigit((unsigned char)chars[*i]) || (chars[*i] == '.' && !is_dot_encountered)))
    {
        // Resize buffer if needed
        if (buffer_index >= buffer_capacity - 1)
        {
            buffer_capacity *= 2;
            char *new_buffer = (char *)realloc(buffer, buffer_capacity);

            if (new_buffer == NULL)
            {
                free(buffer);
                return make_alloc_failure_token();
            }

            buffer = new_buffer;
        }

        if (chars[*i] == '.')
        {
            is_dot_encountered = true;
        }

        buffer[buffer_index++] = chars[(*i)++];
    }

    buffer[buffer_index] = '\0';

    struct Token token;
    if (is_dot_encountered)
    {
        token = make_token(TOKEN_FLOAT, buffer);
    }
    else
    {
        token = make_token(TOKEN_INT, buffer);
    }
    
    free(buffer);

    return token;
}

// Lexer function
struct Token *lexer(char *chars, size_t length)
{
    // Allocate memory for the array of tokens (+1 for EOF token)
    struct Token *tokens = (struct Token *)malloc(sizeof(struct Token) * (length + 1));
    if (tokens == NULL)
    {
        return NULL;
    }

    size_t j = 0;
    for (size_t i = 0; i < length; i++)
    {
        if (chars[i] == '\r' || chars[i] == '\n' || chars[i] == '\0')
        {
            continue;
        }

        if (isdigit((unsigned char)chars[i]))
        {
            tokens[j++] = make_number_token(chars, &i, length);
            i--; // Adjust `i` because the lexer loop will increment `i` again
        }
        else
        {
            char buffer[2] = {chars[i], '\0'};
            tokens[j++] = make_token(TOKEN_STRING, buffer);
        }
    }

    tokens[j++] = make_token(TOKEN_EOF, "\0");

    // Resize tokens to account for skipped characters.
    tokens = realloc(tokens, sizeof(struct Token) * j);
    if (tokens == NULL)
    {
        return NULL;
    }

    return tokens;
}
