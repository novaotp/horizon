const std = @import("std");
const file = @import("./file.zig");
const lexer = @import("./lexer.zig");

pub fn main() !void {
    const sourceCode: []u8 = try file.readFrom("./tests/sample_code/assignment.hor");
    const tokens: []lexer.Token = lexer.tokenize(sourceCode) catch |err| {
        if (err == lexer.LexerError.UnknownCharacter) {
            return std.debug.print("An unknown character was encountered.", .{});
        }

        return err;
    };

    for (tokens) |t| {
        std.debug.print("Token value : {s} | Type : {any}\n", .{ t.value, t.type });
    }
}
