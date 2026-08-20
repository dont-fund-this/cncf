const std = @import("std");
const type_mod = @import("type.zig");

pub fn bind(allocator: std.mem.Allocator, binary_path: []const u8) ?type_mod.Cabi {
    const filename = std.fs.path.basename(binary_path);
    const skips = [_][]const u8{ "c", "cpp", "rust", "go", "swift", "haskell", "zig", "v", "slint_sample" };
    for (skips) |s| {
        if (std.mem.eql(u8, filename, s)) return null;
    }

    var dyn_lib = std.DynLib.open(binary_path) catch return null;

    const pump_sym = dyn_lib.lookup(type_mod.PumpFn, "Pump") orelse {
        dyn_lib.close();
        return null;
    };

    const more_sym = dyn_lib.lookup(type_mod.MoreFn, "More");
    const less_sym = dyn_lib.lookup(type_mod.LessFn, "Less");

    return type_mod.Cabi{
        .name = allocator.dupe(u8, filename) catch filename,
        .path = allocator.dupe(u8, binary_path) catch binary_path,
        .dyn_lib = dyn_lib,
        .more = more_sym,
        .pump = pump_sym,
        .less = less_sym,
    };
}
