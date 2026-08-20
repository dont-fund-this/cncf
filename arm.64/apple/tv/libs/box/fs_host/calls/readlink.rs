use core::ffi::{c_char, c_int};

use super::abi::*;
use super::errno::errno;
use super::errno_to_p9::errno_to_p9;
use super::super::dev::*;
use super::super::file::*;

pub unsafe extern "C" fn boxfs_readlink(
    _fs1: *mut FSDevice,
    buf: *mut c_char,
    buf_size: c_int,
    f: *mut FSFile,
) -> c_int {
    let h = f as *mut Handle;
    let ret = unsafe { readlink((*h).path, buf, (buf_size - 1) as usize) };
    if ret < 0 {
        return -errno_to_p9(errno());
    }
    unsafe { *buf.add(ret as usize) = 0 };
    0
}
