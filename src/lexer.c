#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include "../include/lexer.h"
#include "../include/token.h"

static struct StringTokenType SINGLE_CHAR_TOKENTYPE_MAP[] = {
    "=", TOKEN_ASSIGNMENT,
    ";", TOKEN_SEMICOLON,
    "(", TOKEN_LPAREN,
    ")", TOKEN_LPAREN,
    "{", TOKEN_LBRACE,
    "}", TOKEN_RBRACE,
    "[", TOKEN_LBRACKET,
    "]", TOKEN_RBRACKET,
    "+", TOKEN_PLUS,
    "-", TOKEN_MINUS,
    "*", TOKEN_MULTIPLY,
    "/", TOKEN_DIVIDE,
    "%", TOKEN_MODULO,
    "!", TOKEN_NOT,
};

static struct StringTokenType DOUBLE_CHAR_TOKENTYPE_MAP[] = {
    ">=", TOKEN_GREATEROREQ,
    "<=", TOKEN_LESSOREQ,
    ">", TOKEN_GREATERTHAN,
    "<", TOKEN_LESSTHAN,
    "==", TOKEN_EQUAL,
    "&&", TOKEN_AND,
    "||", TOKEN_OR,
    "**", TOKEN_POWER,
    "//", TOKEN_INTDIVIDE,
};

struct Token check_token_type(const char *chars, size_t *index, size_t length) {
    // Check for double-character tokens first
    size_t double_char_map_length = sizeof(DOUBLE_CHAR_TOKENTYPE_MAP) / sizeof(DOUBLE_CHAR_TOKENTYPE_MAP[0]);
    for (size_t i = 0; i < double_char_map_length; i++) {
        if (*index < length - 1 && strncmp(&chars[*index], DOUBLE_CHAR_TOKENTYPE_MAP[i].str, 2) == 0) {
            *index += 2;  // Move the index forward by 2 characters
            return make_token(DOUBLE_CHAR_TOKENTYPE_MAP[i].type, DOUBLE_CHAR_TOKENTYPE_MAP[i].str);
        }
    }

    // Check for single-character tokens
    size_t single_char_map_length = sizeof(SINGLE_CHAR_TOKENTYPE_MAP) / sizeof(SINGLE_CHAR_TOKENTYPE_MAP[0]);
    for (size_t i = 0; i < single_char_map_length; i++) {
        if (*index < length && strncmp(&chars[*index], SINGLE_CHAR_TOKENTYPE_MAP[i].str, 1) == 0) {
            *index += 1;
            return make_token(SINGLE_CHAR_TOKENTYPE_MAP[i].type, SINGLE_CHAR_TOKENTYPE_MAP[i].str);
        }
    }

    return make_token(TOKEN_EOF, "");
}

static struct Token make_alloc_failure_token()
{
    return (struct Token){TOKEN_EOF, NULL};
}

// Function to handle numbers and create a token for them
static struct Token make_number_token(char *chars, size_t *i, size_t length)
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
        token = make_token(TOKEN_INTEGER, buffer);
    }

    free(buffer);

    return token;
}

static struct Token make_alpha_token(char *chars, size_t *i, size_t length)
{
    /// Initial buffer capacity
    size_t buffer_capacity = 16;
    char *buffer = (char *)malloc(buffer_capacity);
    if (buffer == NULL)
    {
        return make_alloc_failure_token();
    }

    size_t buffer_index = 0;

    while (*i < length && (isalpha((unsigned char)chars[*i]) || chars[*i] == '_'))
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

        buffer[buffer_index++] = chars[(*i)++];
    }

    buffer[buffer_index] = '\0';

    struct Token token;
    if (strcmp("mut", buffer) == 0)
    {
        token = make_token(TOKEN_MUTABLE, buffer);
    }
    else if (strcmp("func", buffer) == 0)
    {
        token = make_token(TOKEN_FUNCTION, buffer);
    }
    else if (strcmp("true", buffer) == 0)
    {
        token = make_token(TOKEN_TRUE, buffer);
    }
    else if (strcmp("false", buffer) == 0)
    {
        token = make_token(TOKEN_FALSE, buffer);
    }
    else if (strcmp("string", buffer) == 0)
    {
        token = make_token(TOKEN_TYPE, buffer);
    }
    else if (strcmp("int", buffer) == 0)
    {
        token = make_token(TOKEN_TYPE, buffer);
    }
    else if (strcmp("float", buffer) == 0)
    {
        token = make_token(TOKEN_TYPE, buffer);
    }
    else if (strcmp("bool", buffer) == 0)
    {
        token = make_token(TOKEN_TYPE, buffer);
    }
    else
    {
        token = make_token(TOKEN_IDENTIFIER, buffer);
    }

    free(buffer);

    return token;
}

static struct Token make_string_token(char *chars, size_t *i, size_t length)
{
    // Skip first " character
    (*i)++;

    /// Initial buffer capacity
    size_t buffer_capacity = 16;
    char *buffer = (char *)malloc(buffer_capacity);
    if (buffer == NULL)
    {
        return make_alloc_failure_token();
    }

    size_t buffer_index = 0;

    while (*i < length && chars[*i] != '"')
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

        buffer[buffer_index++] = chars[(*i)++];
    }

    // Skip last " character
    (*i)++;

    buffer[buffer_index] = '\0';

    struct Token token = make_token(TOKEN_STRING, buffer);

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
        if (isspace(chars[i]))
        {
            continue;
        }

        if (isdigit((unsigned char)chars[i]))
        {
            tokens[j++] = make_number_token(chars, &i, length);
        }
        else if (chars[i] == '"')
        {
            tokens[j++] = make_string_token(chars, &i, length);
            i++;
        }
        else
        {
            struct Token token = check_token_type(chars, &i, length);
            if (token.type != TOKEN_EOF)
            {
                tokens[j++] = token;
            }
            else
            {
                tokens[j++] = make_alpha_token(chars, &i, length);
            }
        }

        i--; // Adjust `i` because the lexer loop will increment `i` again
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
