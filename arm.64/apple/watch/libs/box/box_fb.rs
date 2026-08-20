use core::ffi::c_int;

use crate::fb::{H, PTR, W};

#[no_mangle]
pub extern "C" fn box_fb(w: *mut c_int, h: *mut c_int) -> *const u16 {
    let ww = unsafe { *core::ptr::addr_of!(W) };
    let hh = unsafe { *core::ptr::addr_of!(H) };
    let ptr = unsafe { *core::ptr::addr_of!(PTR) };
    if !w.is_null() {
        unsafe { *w = ww };
    }
    if !h.is_null() {
        unsafe { *h = hh };
    }
    ptr as *const u16
}
