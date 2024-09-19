const std = @import("std");

pub const LexerError = error{UnknownCharacter};

const TokenType = enum { String, Integer, Float, Boolean, AdditionOperator, SubstractionOperator, MultiplicationOperator, DivisionOperator, AssignmentOperator, VarKeyword, ConstKeyword, Identifier, SemiColon, Type, GreaterThan, GreaterOrEqual, Equal, NotEqual, LessOrEqual, LessThan, EOF };
const Token = struct {
    value: []u8,
    type: TokenType,
};

const KEYWORDS = [][]u8{ "var", "const" };

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
        } else if (isArithmeticOperator(currentChar)) {
            newToken = try handleArithmeticOperators(sourceCode, &index);
        } else if (isComparisonOperator(currentChar, sourceCode[index + 1])) {
            newToken = try handleComparisonOperators(sourceCode, &index);
        } else if (isPunctuation(currentChar)) {
            newToken = try handlePunctuations(sourceCode, &index);
        } else if (isString(currentChar)) {
            newToken = try handleStrings(sourceCode, &index);
        } else if (std.ascii.isAlphabetic(currentChar) or currentChar == '_') {
            newToken = try handleIdentifiers(sourceCode, &index);
        } else {
            std.debug.print("Unknown char : {c}\n", .{currentChar});
            return LexerError.UnknownCharacter;
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
    while (std.ascii.isDigit(sourceCode[index.* + i]) or (sourceCode[index.* + i] == '.' and !containsDot)) {
        if (sourceCode[index.* + i] == '.') {
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

fn isArithmeticOperator(char: u8) bool {
    return char == '+' or char == '-' or char == '*' or char == '/' or char == '=';
}

fn handleArithmeticOperators(sourceCode: []u8, index: *usize) !Token {
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    const allocator = gpa.allocator();

    const operator: u8 = sourceCode[index.*];

    var tokenType: TokenType = undefined;
    var buffer: []u8 = try allocator.alloc(u8, 2);
    buffer[1] = 0;

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
    } else if (operator == '=') {
        buffer[0] = '=';
        tokenType = TokenType.AssignmentOperator;
    }

    index.* += 1;

    return Token{ .value = buffer, .type = tokenType };
}

fn isString(char: u8) bool {
    return char == '"';
}

fn handleStrings(sourceCode: []u8, index: *usize) !Token {
    // Skip first "
    index.* += 1;

    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    const allocator = gpa.allocator();

    var buffer: []u8 = try allocator.alloc(u8, 8);

    var i: usize = 0;
    while (sourceCode[index.* + i] != '"') {
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
    // And 1 for the last "
    index.* += i + 1;

    // Resize buffer to account for actual size + 1 for null-terminator.
    buffer = try allocator.realloc(buffer, i + 1);

    return Token{ .value = buffer, .type = TokenType.String };
}

fn handleIdentifiers(sourceCode: []u8, index: *usize) !Token {
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    const allocator = gpa.allocator();

    var buffer: []u8 = try allocator.alloc(u8, 8);

    var i: usize = 0;
    while (std.ascii.isAlphanumeric(sourceCode[index.* + i]) or sourceCode[index.* + i] == '_') {
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

    // Do not include the null-terminator for comparing strings.
    const slicedBuffer = buffer[0..i];

    var tokenType: TokenType = undefined;
    if (std.mem.eql(u8, slicedBuffer, "const")) {
        tokenType = TokenType.ConstKeyword;
    } else if (std.mem.eql(u8, slicedBuffer, "var")) {
        tokenType = TokenType.VarKeyword;
    } else if (isType(slicedBuffer)) {
        tokenType = TokenType.Type;
    } else {
        tokenType = TokenType.Identifier;
    }

    return Token{ .value = buffer, .type = tokenType };
}

fn isPunctuation(char: u8) bool {
    return char == ';';
}

fn handlePunctuations(sourceCode: []u8, index: *usize) !Token {
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    const allocator = gpa.allocator();

    var buffer: []u8 = try allocator.alloc(u8, 2);
    buffer[0] = sourceCode[index.*];
    buffer[1] = 0;

    index.* += 1;

    return Token{ .value = buffer, .type = TokenType.SemiColon };
}

fn isType(buffer: []u8) bool {
    const arr = [4][]const u8{ "string", "int", "float", "bool" };

    for (arr) |item| {
        if (std.mem.eql(u8, buffer, item)) {
            return true;
        }
    }

    return false;
}

fn isComparisonOperator(current: u8, next: u8) bool {
    if (current == '>' or current == '<') {
        if (next == '=') {
            // >= or <=
            return true;
        } else {
            // > or <
            return true;
        }
    } else if ((current == '=' or current == '!') and next == '=') {
        // == or !=
        return true;
    }

    return false;
}

fn handleComparisonOperators(sourceCode: []u8, index: *usize) !Token {
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    const allocator = gpa.allocator();

    var buffer: []u8 = try allocator.alloc(u8, 3);
    var isSingle: bool = false;
    var tokenType: TokenType = undefined;

    const current: u8 = sourceCode[index.*];
    const next: u8 = sourceCode[index.* + 1];
    if (current == '>' or current == '<') {
        if (next == '=') {
            index.* += 2;
            buffer[0] = current;
            buffer[1] = next;

            if (current == '>') {
                tokenType = TokenType.GreaterOrEqual;
            } else {
                tokenType = TokenType.LessOrEqual;
            }
        } else {
            index.* += 1;
            buffer[0] = current;
            isSingle = true;

            if (current == '>') {
                tokenType = TokenType.GreaterThan;
            } else {
                tokenType = TokenType.LessThan;
            }
        }
    } else if ((current == '=' or current == '!') and next == '=') {
        index.* += 2;
        buffer[0] = current;
        buffer[1] = next;

        if (current == '=') {
            tokenType = TokenType.Equal;
        } else {
            tokenType = TokenType.NotEqual;
        }
    }

    if (isSingle) {
        buffer = try allocator.realloc(buffer, 2);
    }

    // Null-terminator
    buffer[buffer.len - 1] = 0;

    return Token{ .value = buffer, .type = tokenType };
}
