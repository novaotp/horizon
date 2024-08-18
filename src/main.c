#include <stdio.h>
#include "../include/token.h"
#include "../include/file.h"
#include "../include/lexer.h"

int main()
{
    char *chars = read_source_file("./tests/sample_code/test1.hor");
    struct Token *tokens = lexer(chars, strlen(chars));

    for (struct Token token = *tokens; token.type != TOKEN_EOF; token = *++tokens)
    {
        printf("Token : %d | Type : %s\n", token.type, token.value);
    }

    return 0;
}
