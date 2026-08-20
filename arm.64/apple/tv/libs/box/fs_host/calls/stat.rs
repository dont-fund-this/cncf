use core::ffi::c_int;

use super::abi::*;
use super::stat_to_qid::stat_to_qid;
use super::super::dev::*;
use super::super::file::*;

pub unsafe extern "C" fn boxfs_stat(
    _fs1: *mut FSDevice,
    f: *mut FSFile,
    st: *mut FSStat,
) -> c_int {
    let h = f as *mut Handle;
    let mut st1: Stat = unsafe { core::mem::zeroed() };
    if unsafe { lstat((*h).path, &mut st1) } != 0 {
        return -P9_ENOENT;
    }
    stat_to_qid(unsafe { core::ptr::addr_of_mut!((*st).qid) }, &st1);
    unsafe {
        (*st).st_mode = st1.st_mode as u32;
        (*st).st_uid = st1.st_uid;
        (*st).st_gid = st1.st_gid;
        (*st).st_nlink = st1.st_nlink as u64;
        (*st).st_rdev = st1.st_rdev as u64;
        (*st).st_size = st1.st_size as u64;
        (*st).st_blksize = st1.st_blksize as u64;
        (*st).st_blocks = st1.st_blocks as u64;
        (*st).st_atime_sec = st1.st_atimespec.tv_sec as u64;
        (*st).st_atime_nsec = st1.st_atimespec.tv_nsec as u32;
        (*st).st_mtime_sec = st1.st_mtimespec.tv_sec as u64;
        (*st).st_mtime_nsec = st1.st_mtimespec.tv_nsec as u32;
        (*st).st_ctime_sec = st1.st_ctimespec.tv_sec as u64;
        (*st).st_ctime_nsec = st1.st_ctimespec.tv_nsec as u32;
    }
    0
}
