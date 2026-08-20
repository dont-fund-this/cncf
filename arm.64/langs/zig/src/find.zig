const std = @import("std");

pub fn find(allocator: std.mem.Allocator, target_dir: ?[]const u8) !std.ArrayList([]const u8) {
    var files: std.ArrayList([]const u8) = .empty;

    var dir_path: []const u8 = "dist";
    if (target_dir) |td| {
        dir_path = td;
    } else if (std.process.getEnvVarOwned(allocator, "DIST_DIR")) |env_dir| {
        dir_path = env_dir;
    } else |_| {
        if (std.fs.cwd().openDir("dist", .{})) |_| {
            dir_path = "dist";
        } else |_| {
            if (std.fs.cwd().openDir("../../dist", .{})) |_| {
                dir_path = "../../dist";
            } else |_| {
                dir_path = "../../../dist";
            }
        }
    }

    var dir = std.fs.cwd().openDir(dir_path, .{ .iterate = true }) catch return files;
    defer dir.close();

    var iter = dir.iterate();
    while (try iter.next()) |entry| {
        if (entry.kind == .file and !std.mem.eql(u8, entry.name, ".DS_Store")) {
            const p = try std.fs.path.join(allocator, &[_][]const u8{ dir_path, entry.name });
            try files.append(allocator, p);
        }
    }
    return files;
}
