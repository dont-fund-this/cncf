use core::ffi::{c_char, c_int};

use super::abi::*;
use super::errno::errno;
use super::errno_to_p9::errno_to_p9;
use super::is_ro::is_ro;
use super::super::dev::*;
use super::super::file::*;

pub unsafe extern "C" fn boxfs_unlinkat(
    fs1: *mut FSDevice,
    f: *mut FSFile,
    name: *const c_char,
) -> c_int {
    if is_ro(fs1) {
        return -P9_EPERM;
    }
    let h = f as *mut Handle;
    let path = compose_path(unsafe { (*h).path }, name);
    let ret = unsafe { remove(path) };
    free_c(path);
    if ret < 0 {
        -errno_to_p9(errno())
    } else {
        0
    }
}
