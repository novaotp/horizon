const std = @import("std");
const file = @import("./file.zig");
const lexer = @import("./lexer.zig");

pub fn main() !void {
    const sourceCode = try file.readFrom("./tests/sample_code/addition.hor");
    const tokens = try lexer.tokenize(sourceCode);

    for (tokens) |t| {
        std.debug.print("Token value : {s} | Type : {any}\n", .{ t.value, t.type });
    }
}
