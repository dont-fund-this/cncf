use core::ffi::c_char;

use super::super::dev::FSDevice;
use super::super::holder::{slot_of, SLOTS};

pub fn root_ptr(fs1: *mut FSDevice) -> *const c_char {
    match slot_of(fs1) {
        Some(i) => unsafe { core::ptr::addr_of!(SLOTS[i].root) as *const c_char },
        None => core::ptr::null(),
    }
}
