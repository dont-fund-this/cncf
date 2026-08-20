use core::ffi::{c_char, c_void};

extern "C" {
    fn malloc(size: usize) -> *mut c_void;
}

pub fn dup(s: &str) -> *mut c_char {
    let bytes = s.as_bytes();
    let n = bytes.len();
    let p = unsafe { malloc(n + 1) } as *mut u8;
    if p.is_null() {
        return core::ptr::null_mut();
    }
    unsafe {
        core::ptr::copy_nonoverlapping(bytes.as_ptr(), p, n);
        *p.add(n) = 0;
    }
    p as *mut c_char
}
