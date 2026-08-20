use core::ffi::{c_char, c_int};
use crate::cstr::cstr;

extern "C" {
    fn mkdir(path: *const c_char, mode: u16) -> c_int;
}

pub fn dirs(dest: &str) {
    let bytes = dest.as_bytes();
    for i in 1..bytes.len() {
        if bytes[i] == b'/' {
            unsafe { mkdir(cstr(&dest[..i]).as_ptr() as *const c_char, 0o755) };
        }
    }
}
