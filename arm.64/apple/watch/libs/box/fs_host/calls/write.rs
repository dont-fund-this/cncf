use core::ffi::{c_int, c_void};

use super::abi::*;
use super::errno::errno;
use super::errno_to_p9::errno_to_p9;
use super::is_ro::is_ro;
use super::super::dev::*;
use super::super::file::*;

pub unsafe extern "C" fn boxfs_write(
    fs1: *mut FSDevice,
    f: *mut FSFile,
    offset: u64,
    buf: *const u8,
    count: c_int,
) -> c_int {
    if is_ro(fs1) {
        return -P9_EPERM;
    }
    let h = f as *mut Handle;
    if unsafe { !(*h).is_opened || (*h).is_dir } {
        return -P9_EPROTO;
    }
    if count < 0 {
        return -P9_EPROTO;
    }
    let ret = unsafe { pwrite((*h).fd, buf as *const c_void, count as usize, offset as i64) };
    if ret < 0 {
        -errno_to_p9(errno())
    } else {
        ret as c_int
    }
}
