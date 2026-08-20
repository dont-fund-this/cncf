const std = @import("std");

pub const Address = ?[*:0]const u8;
pub const Payload = ?[*:0]const u8;
pub const Options = ?[*:0]const u8;
pub const Sid = ?[*:0]const u8;
pub const Tag = ?[*:0]const u8;

pub const FitFn = *const fn (address: Address, payload: Payload, options: Options) callconv(.c) bool;
pub const FunFn = *const fn (address: Address, payload: Payload, options: Options) callconv(.c) c_int;

pub const Def = extern struct {
    sid: Sid,
    tag: Tag,
    fit: ?FitFn,
    fun: ?FunFn,
};

pub const MoreFn = *const fn (def: *const Def) callconv(.c) c_int;
pub const PumpFn = *const fn (address: Address, payload: Payload, options: Options) callconv(.c) c_int;
pub const LessFn = *const fn (def: *const Def) callconv(.c) c_int;

pub const Cabi = struct {
    name: []const u8,
    path: []const u8,
    dyn_lib: std.DynLib,
    more: ?MoreFn,
    pump: PumpFn,
    less: ?LessFn,
};

pub const Triplet = struct {
    address: [:0]const u8,
    payload: [:0]const u8,
    options: [:0]const u8,
};
