const type_mod = @import("type.zig");

pub fn trip() [3]type_mod.Triplet {
    return [_]type_mod.Triplet{
        .{ .address = "/version", .payload = "{}", .options = "{\"once\":true}" },
        .{ .address = "/storage", .payload = "{}", .options = "{\"once\":true}" },
        .{ .address = "sql.help", .payload = "{}", .options = "{\"once\":true}" },
    };
}
