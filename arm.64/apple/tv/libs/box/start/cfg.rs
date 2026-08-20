use alloc::string::String;
use alloc::vec::Vec;
use core::ffi::{c_char, c_int};

use super::text::text;
use super::num::num;
use super::resolve::resolve;
use super::mounts::{mounts, Mount};
use super::fbdim::fbdim;

pub struct Cfg {
    pub bios: String,
    pub kernel: String,
    pub drive: String,
    pub cmdline: String,
    pub ram: u64,
    pub mounts: Vec<Mount>,
    pub fb_w: c_int,
    pub fb_h: c_int,
}

pub fn cfg(payload: *const c_char) -> Cfg {
    let (fb_w, fb_h) = fbdim(payload);
    Cfg {
        bios: resolve(text(payload, "bios").unwrap_or_else(|| nul(""))),
        kernel: resolve(text(payload, "kernel").unwrap_or_else(|| nul(""))),
        drive: resolve(text(payload, "drive").unwrap_or_else(|| nul(""))),
        cmdline: text(payload, "cmdline").unwrap_or_else(|| nul("console=hvc0 root=/dev/vda rw")),
        ram: num(payload, "ram", 32),
        mounts: mounts(payload),
        fb_w,
        fb_h,
    }
}

fn nul(s: &str) -> String {
    let mut c: String = s.into();
    c.push('\0');
    c
}
