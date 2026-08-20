use core::ffi::{c_char, c_int};

use super::abi::*;
use super::close::boxfs_close;
use super::errno::errno;
use super::errno_to_p9::errno_to_p9;
use super::is_ro::is_ro;
use super::p9_flags_to_host::p9_flags_to_host;
use super::stat_to_qid::stat_to_qid;
use super::super::dev::*;
use super::super::file::*;

pub unsafe extern "C" fn boxfs_create(
    fs1: *mut FSDevice,
    qid: *mut FSQID,
    f: *mut FSFile,
    name: *const c_char,
    flags: u32,
    mode: u32,
    _gid: u32,
) -> c_int {
    if is_ro(fs1) {
        return -P9_EPERM;
    }
    let h = f as *mut Handle;
    let mut st: Stat = unsafe { core::mem::zeroed() };
    unsafe { boxfs_close(fs1, f) };
    let path = compose_path(unsafe { (*h).path }, name);
    let fd = unsafe { open(path, p9_flags_to_host(flags as c_int) | O_CREAT | O_NOFOLLOW, mode as c_int) };
    if fd < 0 {
        free_c(path);
        return -errno_to_p9(errno());
    }
    if unsafe { lstat(path, &mut st) } != 0 {
        free_c(path);
        unsafe { close(fd) };
        return -errno_to_p9(errno());
    }
    free_c(unsafe { (*h).path });
    unsafe {
        (*h).path = path;
        (*h).is_opened = true;
        (*h).is_dir = false;
        (*h).fd = fd;
    }
    stat_to_qid(qid, &st);
    0
}
