use alloc::vec::Vec;
use core::ffi::{c_char, c_int, c_void};

extern "C" {
    fn fopen(path: *const c_char, mode: *const c_char) -> *mut c_void;
    fn fseek(stream: *mut c_void, off: i64, whence: c_int) -> c_int;
    fn ftell(stream: *mut c_void) -> i64;
    fn fread(ptr: *mut c_void, size: usize, nmemb: usize, stream: *mut c_void) -> usize;
    fn fclose(stream: *mut c_void) -> c_int;
}

pub fn read(path: *const c_char) -> Option<Vec<u8>> {
    let f = unsafe { fopen(path, c"rb".as_ptr()) };
    if f.is_null() {
        return None;
    }
    unsafe { fseek(f, 0, 2) };
    let n = unsafe { ftell(f) };
    unsafe { fseek(f, 0, 0) };
    if n <= 0 {
        unsafe { fclose(f) };
        return None;
    }
    let len = n as usize;
    let mut buf: Vec<u8> = Vec::with_capacity(len);
    let got = unsafe { fread(buf.as_mut_ptr() as *mut c_void, 1, len, f) };
    unsafe { fclose(f) };
    if got != len {
        return None;
    }
    unsafe { buf.set_len(len) };
    Some(buf)
}
