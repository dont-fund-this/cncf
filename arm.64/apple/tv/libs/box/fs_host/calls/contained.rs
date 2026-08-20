use core::ffi::{c_char, c_void};

use super::abi::*;

pub fn contained(path: *const c_char, root: *const c_char) -> bool {
    let rp = unsafe { realpath(path, core::ptr::null_mut()) };
    if rp.is_null() {
        return false;
    }
    let rr = unsafe { realpath(root, core::ptr::null_mut()) };
    if rr.is_null() {
        unsafe { free(rp as *mut c_void) };
        return false;
    }
    let rp_len = unsafe { strlen(rp) };
    let rr_len = unsafe { strlen(rr) };
    let mut ok = rp_len >= rr_len;
    let mut k = 0;
    while ok && k < rr_len {
        if unsafe { *rp.add(k) } != unsafe { *rr.add(k) } {
            ok = false;
        }
        k += 1;
    }
    if ok && rp_len > rr_len {
        ok = unsafe { *rp.add(rr_len) } == b'/' as c_char;
    }
    unsafe {
        free(rp as *mut c_void);
        free(rr as *mut c_void);
    }
    ok
}
