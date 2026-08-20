use core::ffi::{c_char, c_int};

use super::abi::*;
use super::errno::errno;
use super::errno_to_p9::errno_to_p9;
use super::root_ptr::root_ptr;
use super::stat_to_qid::stat_to_qid;
use super::super::dev::*;
use super::super::file::*;

pub unsafe extern "C" fn boxfs_attach(
    fs1: *mut FSDevice,
    pf: *mut *mut FSFile,
    qid: *mut FSQID,
    uid: u32,
    _uname: *const c_char,
    _aname: *const c_char,
) -> c_int {
    let root = root_ptr(fs1);
    let mut st: Stat = unsafe { core::mem::zeroed() };
    if root.is_null() || unsafe { lstat(root, &mut st) } != 0 {
        unsafe { *pf = core::ptr::null_mut() };
        return -errno_to_p9(errno());
    }
    unsafe { *pf = fid_create(dup_c(root), uid) as *mut FSFile };
    stat_to_qid(qid, &st);
    0
}
