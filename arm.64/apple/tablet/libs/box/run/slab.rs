use core::ffi::{c_char, c_int, c_void};

extern "C" {
    fn fopen(path: *const c_char, mode: *const c_char) -> *mut c_void;
    fn fseek(stream: *mut c_void, off: i64, whence: c_int) -> c_int;
    fn ftell(stream: *mut c_void) -> i64;
    fn fread(ptr: *mut c_void, size: usize, nmemb: usize, stream: *mut c_void) -> usize;
    fn fclose(stream: *mut c_void) -> c_int;
    fn malloc(size: usize) -> *mut c_void;
    fn free(ptr: *mut c_void);
}

pub fn slab(path: *const c_char) -> Option<(*mut u8, c_int)> {
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
    let p = unsafe { malloc(len) } as *mut u8;
    if p.is_null() {
        unsafe { fclose(f) };
        return None;
    }
    let got = unsafe { fread(p as *mut c_void, 1, len, f) };
    unsafe { fclose(f) };
    if got != len {
        unsafe { free(p as *mut c_void) };
        return None;
    }
    Some((p, len as c_int))
}
