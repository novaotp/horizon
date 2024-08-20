#include "../include/parser.h"

Expression *parse_primitive(const Token *tokens, size_t *i, size_t length) {
    Token current_token = tokens[(*i)++];

    Expression *expr = (Expression *)malloc(sizeof(Expression));
    if (!expr) {
        return NULL;  // Handle memory allocation failure
    }

    switch (current_token.type) {
        case TOKEN_INTEGER: {
            IntLiteral *int_literal = (IntLiteral *)malloc(sizeof(IntLiteral));
            if (!int_literal) {
                free(expr);
                return NULL;  // Handle memory allocation failure
            }

            int_literal->value = (uint32_t)atoi(current_token.value);
            expr->literal_value = malloc(sizeof(Literal));
            expr->literal_value->int_literal = int_literal;

            break;
        }
        case TOKEN_FLOAT: {
            FloatLiteral *float_literal = (FloatLiteral *)malloc(sizeof(FloatLiteral));
            if (!float_literal) {
                free(expr);
                return NULL;  // Handle memory allocation failure
            }

            float_literal->value = (float)atof(current_token.value);
            expr->literal_value = malloc(sizeof(Literal));
            expr->literal_value->float_literal = float_literal;

            break;
        }
        case TOKEN_STRING: {
            StringLiteral *string_literal = (StringLiteral *)malloc(sizeof(StringLiteral));
            if (!string_literal) {
                free(expr);
                return NULL;  // Handle memory allocation failure
            }

            string_literal->value = strdup(current_token.value);
            string_literal->length = strlen(current_token.value);
            expr->literal_value = malloc(sizeof(Literal));
            expr->literal_value->string_literal = string_literal;

            break;
        }
        case TOKEN_BOOLEAN: {
            BooleanLiteral *boolean_literal = (BooleanLiteral *)malloc(sizeof(BooleanLiteral));
            if (!boolean_literal) {
                free(expr);
                return NULL;  // Handle memory allocation failure
            }

            boolean_literal->value = (strcmp(current_token.value, "true") == 0);
            expr->literal_value = malloc(sizeof(Literal));
            expr->literal_value->boolean_literal = boolean_literal;

            break;
        }
        default:
            free(expr);
            return NULL;  // Handle unsupported token types
    }

    return expr;
}

Expression *parse_addition(const Token *tokens, size_t *i, size_t length)
{
    return parse_primitive(tokens, i, length);
}

Expression *parse_multiplication(const Token *tokens, size_t *i, size_t length)
{
    return parse_addition(tokens, i, length);
}

Expression *parse_expression(const Token *tokens, size_t *i, size_t length)
{
    return parse_multiplication(tokens, i, length);
}

StatementOrExpression *parse(const Token *tokens, size_t *i, size_t length)
{
    StatementOrExpression *stmt_or_expr = (StatementOrExpression *)malloc(sizeof(StatementOrExpression));
    if (!stmt_or_expr) {
        return NULL;
    }

    stmt_or_expr->expression = parse_expression(tokens, i, length);

    return stmt_or_expr;
}

AbstractSyntaxTree parser(const struct Token *tokens, size_t length)
{
    size_t capacity = 16;
    AbstractSyntaxTree ast;
    ast.statements = malloc(sizeof(StatementOrExpression) * capacity);
    if (!ast.statements) {
        ast.statements = NULL;
        return ast;
    }

    size_t i = 0;
    size_t stmt_count = 0;
    while (i < length && tokens[i].type != TOKEN_EOF)
    {
        if (stmt_count >= capacity) {
            capacity *= 2;
            ast.statements = realloc(ast.statements, sizeof(StatementOrExpression) * capacity);
            if (!ast.statements) {
                return ast;
            }
        }

        ast.statements[stmt_count++] = *parse(tokens, &i, length);
    }

    ast.statements = realloc(ast.statements, sizeof(StatementOrExpression) * stmt_count);

    return ast;
}
