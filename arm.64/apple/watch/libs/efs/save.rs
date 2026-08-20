use core::ffi::{c_char, c_int, c_void};
use crate::cstr::cstr;

extern "C" {
    fn creat(path: *const c_char, mode: u16) -> c_int;
    fn write(fd: c_int, buf: *const c_void, count: usize) -> isize;
    fn close(fd: c_int) -> c_int;
}

pub fn save(path: &str, bytes: &[u8]) -> bool {
    let fd = unsafe { creat(cstr(path).as_ptr() as *const c_char, 0o644) };
    if fd < 0 {
        return false;
    }
    let mut off = 0usize;
    while off < bytes.len() {
        let n = unsafe { write(fd, bytes[off..].as_ptr() as *const c_void, bytes.len() - off) };
        if n <= 0 {
            unsafe { close(fd) };
            return false;
        }
        off += n as usize;
    }
    unsafe { close(fd) };
    true
}
