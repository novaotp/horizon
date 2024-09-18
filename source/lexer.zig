const std = @import("std");

const TokenType = enum { String, Integer, Float, Boolean, AdditionOperator, SubstractionOperator, MultiplicationOperator, DivisionOperator, EOF };
const Token = struct {
    value: []u8,
    type: TokenType,
};

/// Tokenizes a given source code.
pub fn tokenize(sourceCode: []u8) ![]Token {
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    const allocator = gpa.allocator();

    var tokens: []Token = try allocator.alloc(Token, 16);
    var tokenCount: usize = 0;

    var index: usize = 0;
    while (index < sourceCode.len) {
        if (tokenCount == tokens.len) {
            tokens = try allocator.realloc(tokens, tokens.len * 2);
        }

        const currentChar: u8 = sourceCode[index];
        var newToken: Token = undefined;

        if (std.ascii.isDigit(currentChar)) {
            newToken = try handleNumbers(sourceCode, &index);
        } else if (std.ascii.isWhitespace(currentChar)) {
            index += 1;
            continue;
        } else if (isOperator(currentChar)) {
            newToken = try handleOperators(sourceCode, &index);
        } else {
            var value: []u8 = try allocator.alloc(u8, 2);
            value[0] = currentChar;
            value[1] = 0;

            newToken = Token{ .value = value, .type = TokenType.String };
            index += 1;
        }

        tokens[tokenCount] = newToken;
        tokenCount += 1;
    }

    if (tokenCount == tokens.len) {
        tokens = try allocator.realloc(tokens, tokens.len + 1);
    }

    tokens[tokenCount] = Token{ .value = "", .type = TokenType.EOF };
    tokenCount += 1;

    return tokens[0..tokenCount];
}

fn handleNumbers(sourceCode: []u8, index: *usize) !Token {
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    const allocator = gpa.allocator();

    var buffer: []u8 = try allocator.alloc(u8, 8);
    var containsDot: bool = false;

    var i: usize = 0;
    while (std.ascii.isDigit(sourceCode[i]) or (sourceCode[i] == '.' and !containsDot)) {
        if (sourceCode[i] == '.') {
            containsDot = true;
        }

        // Reallocate if the buffer is full
        if (buffer.len == i) {
            buffer = try allocator.realloc(buffer, buffer.len * 2);
        }

        buffer[i] = sourceCode[index.* + i];
        i += 1;
    }

    // Add space for null-terminator
    if (i == buffer.len) {
        buffer = try allocator.realloc(buffer, buffer.len + 1);
    }

    buffer[i] = 0;

    // Update global index by the amount we advanced.
    index.* += i;

    // Resize buffer to account for actual size + 1 for null-terminator.
    buffer = try allocator.realloc(buffer, i + 1);

    const tokenType: TokenType = if (containsDot) TokenType.Float else TokenType.Integer;
    return Token{ .value = buffer, .type = tokenType };
}

fn isOperator(char: u8) bool {
    return char == '+' or char == '-' or char == '*' or char == '/';
}

fn handleOperators(sourceCode: []u8, index: *usize) !Token {
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    const allocator = gpa.allocator();

    const operator: u8 = sourceCode[index.*];

    var tokenType: TokenType = undefined;
    var buffer: []u8 = try allocator.alloc(u8, 2);
    if (operator == '+') {
        buffer[0] = '+';
        tokenType = TokenType.AdditionOperator;
    } else if (operator == '-') {
        buffer[0] = '-';
        tokenType = TokenType.SubstractionOperator;
    } else if (operator == '*') {
        buffer[0] = '*';
        tokenType = TokenType.MultiplicationOperator;
    } else if (operator == '/') {
        buffer[0] = '/';
        tokenType = TokenType.DivisionOperator;
    }

    buffer[1] = 0;
    index.* += 1;

    return Token{ .value = buffer, .type = tokenType };
}
