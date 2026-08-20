use alloc::string::String;
use alloc::vec::Vec;
use core::ffi::{c_char, c_int, c_void, CStr};
use super::cstr::cstr;
use super::join::join;
use super::type_::Dirent;

extern "C" {
    fn opendir(path: *const u8) -> *mut c_void;
    fn readdir(dir: *mut c_void) -> *const Dirent;
    fn closedir(dir: *mut c_void) -> c_int;
}

pub fn read(dir: &str) -> Vec<String> {
    let mut out = Vec::new();
    let dirp = unsafe { opendir(cstr(dir).as_ptr()) };
    if dirp.is_null() {
        return out;
    }
    loop {
        let e = unsafe { readdir(dirp) };
        if e.is_null() {
            break;
        }
        let name = unsafe { CStr::from_ptr(core::ptr::addr_of!((*e).d_name).cast::<c_char>()) };
        out.push(join(dir, name.to_str().unwrap_or("")));
    }
    unsafe { closedir(dirp) };
    out
}
