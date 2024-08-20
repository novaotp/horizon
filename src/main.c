#include <stdio.h>
#include "../include/token.h"
#include "../include/file.h"
#include "../include/lexer.h"
#include "../include/parser.h"

int main()
{
    char *chars = read_source_file("./tests/sample_code/test4.hor");
    Token *tokens = lexer(chars, strlen(chars));

    AbstractSyntaxTree ast = parser(tokens, sizeof(tokens) / sizeof(tokens[0]));

    printf("%s", ast);

    return 0;
}
