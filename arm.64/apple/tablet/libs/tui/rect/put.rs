use core::ffi::{c_int, c_void};

extern "C" {
    fn write(fd: c_int, buf: *const c_void, count: usize) -> isize;
}

pub fn put(s: &str) {
    unsafe { write(1, s.as_ptr() as *const c_void, s.len()) };
}
