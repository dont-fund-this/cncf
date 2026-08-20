use core::ffi::{c_char, c_int, c_void};

extern "C" {
    fn dlopen(path: *const c_char, mode: c_int) -> *mut c_void;
    fn dlclose(handle: *mut c_void) -> c_int;
}

pub unsafe fn open_lib(path: *const c_char) -> *mut c_void {
    if path.is_null() {
        return core::ptr::null_mut();
    }
    dlopen(path, 1)
}

pub unsafe fn close_lib(handle: *mut c_void) -> i32 {
    if handle.is_null() {
        return -1;
    }
    dlclose(handle)
}
