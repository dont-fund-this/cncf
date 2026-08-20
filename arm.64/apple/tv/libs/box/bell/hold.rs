use core::ffi::c_void;

use crate::emu::PhysMemoryRange;

pub static mut PTR: *mut c_void = core::ptr::null_mut();
pub static mut PR: *mut PhysMemoryRange = core::ptr::null_mut();
pub static mut SIZE: u64 = 0;
pub static mut SEEN: u32 = 0;
