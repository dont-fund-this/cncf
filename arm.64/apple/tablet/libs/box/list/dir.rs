use core::ffi::{c_char, c_int, c_void};
use crate::emu::Dirent;

extern "C" {
    fn opendir(path: *const c_char) -> *mut c_void;
    fn readdir(dir: *mut c_void) -> *const Dirent;
    fn closedir(dir: *mut c_void) -> c_int;
    fn puts(s: *const c_char) -> c_int;
}

pub fn dir(path: *const c_char) -> c_int {
    let mut count: c_int = 0;
    let dirp = unsafe { opendir(path) };
    if dirp.is_null() {
        return 0;
    }
    loop {
        let e = unsafe { readdir(dirp) };
        if e.is_null() {
            break;
        }
        unsafe { puts(core::ptr::addr_of!((*e).d_name).cast::<c_char>()) };
        count += 1;
    }
    unsafe { closedir(dirp) };
    count
}
