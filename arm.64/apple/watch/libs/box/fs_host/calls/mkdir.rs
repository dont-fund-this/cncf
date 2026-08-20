use core::ffi::{c_char, c_int};

use super::abi::*;
use super::errno::errno;
use super::errno_to_p9::errno_to_p9;
use super::is_ro::is_ro;
use super::stat_to_qid::stat_to_qid;
use super::super::dev::*;
use super::super::file::*;

pub unsafe extern "C" fn boxfs_mkdir(
    fs1: *mut FSDevice,
    qid: *mut FSQID,
    f: *mut FSFile,
    name: *const c_char,
    mode: u32,
    _gid: u32,
) -> c_int {
    if is_ro(fs1) {
        return -P9_EPERM;
    }
    let h = f as *mut Handle;
    let path = compose_path(unsafe { (*h).path }, name);
    let mut st: Stat = unsafe { core::mem::zeroed() };
    if unsafe { mkdir(path, mode as u16) } < 0 {
        free_c(path);
        return -errno_to_p9(errno());
    }
    if unsafe { lstat(path, &mut st) } != 0 {
        free_c(path);
        return -errno_to_p9(errno());
    }
    free_c(path);
    stat_to_qid(qid, &st);
    0
}
