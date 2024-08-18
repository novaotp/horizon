#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#ifndef TOKEN_H
#define TOKEN_H

enum TokenType
{
    // Parens
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_LBRACE,
    TOKEN_RBRACE,
    TOKEN_LBRACKET,
    TOKEN_RBRACKET,

    // Operations
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_MULTIPLY,
    TOKEN_DIVIDE,
    TOKEN_POWER,
    TOKEN_MODULO,
    TOKEN_INTDIVIDE,

    TOKEN_AND,
    TOKEN_OR,
    TOKEN_NOT,

    TOKEN_GREATEROREQ,
    TOKEN_GREATERTHAN,
    TOKEN_LESSOREQ,
    TOKEN_LESSTHAN,
    TOKEN_EQUAL,

    // Literals
    TOKEN_IDENTIFIER,

    // Value types
    TOKEN_STRING,
    TOKEN_INTEGER,
    TOKEN_FLOAT,
    TOKEN_BOOLEAN,

    // Punctuation
    TOKEN_DOT,
    TOKEN_COMMA,
    TOKEN_COLON,
    TOKEN_SEMICOLON,
    TOKEN_ASSIGNMENT,

    // Conditionals
    TOKEN_IF,
    TOKEN_ELSEIF,
    TOKEN_ELSE,
    TOKEN_SWITCH,
    TOKEN_CASE,

    // Loops
    TOKEN_WHILE,
    TOKEN_FOR,
    TOKEN_FOREACH,

    // Keywords
    TOKEN_TRUE,
    TOKEN_FALSE,
    TOKEN_MUTABLE,
    TOKEN_FUNCTION,
    TOKEN_TYPE,
    
    TOKEN_EOF
};

struct Token
{
    enum TokenType type;
    char *value;
};

struct Token make_token(enum TokenType type, const char value[]);

#endif // TOKEN_H
