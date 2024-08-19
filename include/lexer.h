#include "./token.h"
#include <ctype.h>

#ifndef LEXER_H
#define LEXER_H

struct Token *lexer(char *chars, size_t length);

#endif
