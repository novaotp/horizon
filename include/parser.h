#include <stdbool.h>
#include <ctype.h>
#include <stdint.h>
#include "../include/token.h"

#ifndef PARSER_H
#define PARSER_H

typedef struct AbstractSyntaxTree AbstractSyntaxTree;
typedef union StatementOrExpression StatementOrExpression;
typedef union Statement Statement;
typedef union Expression Expression;
typedef struct VariableAssignmentStatement VariableAssignmentStatement;
typedef struct BinaryOperationExpression BinaryOperationExpression;
typedef struct IfElseExpression IfElseExpression;
typedef struct IfBranchExpression IfBranchExpression;

typedef union Literal Literal;
typedef struct StringLiteral StringLiteral;
typedef struct IntLiteral IntLiteral;
typedef struct FloatLiteral FloatLiteral;
typedef struct BooleanLiteral BooleanLiteral;

struct AbstractSyntaxTree
{
    union StatementOrExpression *statements;
};

union StatementOrExpression
{
    Statement *statement;
    Expression *expression;
};

union Statement
{
    VariableAssignmentStatement *variable_assignment;
};

struct VariableAssignmentStatement
{
    bool is_mutable;
    char *type;
    char *identifier;
    Expression *value;
};

union Expression
{
    Literal *literal_value;
    BinaryOperationExpression *binary_operation;
};

struct BinaryOperationExpression
{
    Expression *left;
    char *operator;
    Expression *right;
};

struct IfElseExpression
{
    IfBranchExpression *if_condition;
    IfBranchExpression *elseif_condition;
    IfBranchExpression *else_condition;
};

struct IfBranchExpression
{
    Expression *condition;
    StatementOrExpression *body;
};

union Literal
{
    StringLiteral *string_literal;
    IntLiteral *int_literal;
    FloatLiteral *float_literal;
    BooleanLiteral *boolean_literal;
};

struct StringLiteral
{
    char *value;
    size_t length;
};

struct IntLiteral
{
    uint32_t value;
};

struct FloatLiteral
{
    float value;
};

struct BooleanLiteral
{
    bool value;
};

AbstractSyntaxTree parser(const Token tokens[], size_t length);

#endif
