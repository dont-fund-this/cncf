use core::ffi::{c_char, c_int};

use super::abi::*;
use super::errno::errno;
use super::errno_to_p9::errno_to_p9;
use super::is_ro::is_ro;
use super::super::dev::*;
use super::super::file::*;

pub unsafe extern "C" fn boxfs_link(
    fs1: *mut FSDevice,
    df: *mut FSFile,
    f: *mut FSFile,
    name: *const c_char,
) -> c_int {
    if is_ro(fs1) {
        return -P9_EPERM;
    }
    let dh = df as *mut Handle;
    let h = f as *mut Handle;
    let path = compose_path(unsafe { (*dh).path }, name);
    if unsafe { link((*h).path, path) } < 0 {
        free_c(path);
        return -errno_to_p9(errno());
    }
    free_c(path);
    0
}
