const std = @import("std");

pub fn readFrom(path: []const u8) ![]u8 {
    const allocator = std.heap.page_allocator;
    const file = try std.fs.cwd().openFile(path, .{});
    defer file.close();

    const fileSize = (try file.stat()).size;
    const buffer = try allocator.alloc(u8, fileSize);

    try file.reader().readNoEof(buffer);

    return buffer;
}
