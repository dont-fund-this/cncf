use core::ffi::{c_char, c_int, c_void};
use crate::cstr::cstr;
use crate::path::path;
use crate::parse::parse;
use crate::pool::{hold, ready};

extern "C" {
    fn open(path: *const c_char, flags: c_int) -> c_int;
    fn lseek(fd: c_int, off: i64, whence: c_int) -> i64;
    fn mmap(addr: *mut c_void, len: usize, prot: c_int, flags: c_int, fd: c_int, offset: i64) -> *mut c_void;
    fn close(fd: c_int) -> c_int;
}

pub fn map() -> bool {
    if ready() {
        return true;
    }
    let mut p = match path() {
        Some(d) => d,
        None => return false,
    };
    p.push_str("/Frameworks/libefs.bin");
    let fd = unsafe { open(cstr(&p).as_ptr() as *const c_char, 0) };
    if fd < 0 {
        return false;
    }
    let len = unsafe { lseek(fd, 0, 2) };
    if len <= 0 {
        unsafe { close(fd) };
        return false;
    }
    let addr = unsafe { mmap(core::ptr::null_mut(), len as usize, 1, 2, fd, 0) };
    unsafe { close(fd) };
    if addr as isize == -1 {
        return false;
    }
    let bytes = unsafe { core::slice::from_raw_parts(addr as *const u8, len as usize) };
    match parse(bytes) {
        Some((blobs, entries)) => { hold(addr as *const u8, blobs, entries); true }
        None => false,
    }
}
