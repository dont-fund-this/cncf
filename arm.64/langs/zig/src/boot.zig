const std = @import("std");
const type_mod = @import("type.zig");
const find_mod = @import("find.zig");
const bind_mod = @import("bind.zig");

pub fn boot(allocator: std.mem.Allocator, target_dir: ?[]const u8) !std.ArrayList(type_mod.Cabi) {
    var engines: std.ArrayList(type_mod.Cabi) = .empty;

    if (std.process.getEnvVarOwned(allocator, "PAT_LIB")) |env_lib| {
        defer allocator.free(env_lib);
        if (env_lib.len > 0) {
            if (bind_mod.bind(allocator, env_lib)) |c| {
                try engines.append(allocator, c);
                return engines;
            }
        }
    } else |_| {}

    var files = try find_mod.find(allocator, target_dir);
    defer files.deinit(allocator);

    for (files.items) |file| {
        if (bind_mod.bind(allocator, file)) |c| {
            try engines.append(allocator, c);
        }
        allocator.free(file);
    }
    return engines;
}
