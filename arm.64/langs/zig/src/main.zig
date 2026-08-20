const std = @import("std");
const boot_mod = @import("boot.zig");
const trip_mod = @import("trip.zig");

pub fn main() !void {
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    const allocator = gpa.allocator();
    defer _ = gpa.deinit();

    var args = try std.process.argsWithAllocator(allocator);
    defer args.deinit();
    _ = args.next(); // exe
    const target_dir = args.next();

    var dist = try boot_mod.boot(allocator, target_dir);
    defer {
        for (dist.items) |*d| {
            d.dyn_lib.close();
            allocator.free(d.name);
            allocator.free(d.path);
        }
        dist.deinit(allocator);
    }

    if (dist.items.len > 0) {
        const trips = trip_mod.trip();
        for (dist.items) |d| {
            for (trips) |t| {
                _ = d.pump(t.address.ptr, t.payload.ptr, t.options.ptr);
            }
        }
    }

    std.debug.print(
        \\{{
        \\  "lang": "zig",
        \\  "status": "ready",
        \\  "engines": {d}
        \\}}
        \\
    , .{dist.items.len});
}
