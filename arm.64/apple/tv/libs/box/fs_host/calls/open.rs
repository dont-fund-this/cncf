use core::ffi::{c_int, c_void};

use super::abi::*;
use super::close::boxfs_close;
use super::errno::errno;
use super::errno_to_p9::errno_to_p9;
use super::is_ro::is_ro;
use super::p9_flags_to_host::p9_flags_to_host;
use super::stat_to_qid::stat_to_qid;
use super::super::dev::*;
use super::super::file::*;

pub unsafe extern "C" fn boxfs_open(
    fs1: *mut FSDevice,
    qid: *mut FSQID,
    f: *mut FSFile,
    flags: u32,
    _cb: Option<FSOpenCompletionFunc>,
    _opaque: *mut c_void,
) -> c_int {
    let h = f as *mut Handle;
    let mut st: Stat = unsafe { core::mem::zeroed() };
    unsafe { boxfs_close(fs1, f) };
    if unsafe { lstat((*h).path, &mut st) } != 0 {
        return -errno_to_p9(errno());
    }
    if st.st_mode & S_IFMT == S_IFLNK {
        return -P9_EPERM;
    }
    if is_ro(fs1) && flags as c_int & (P9_O_NOACCESS | P9_O_TRUNC) != 0 {
        return -P9_EPERM;
    }
    stat_to_qid(qid, &st);
    if flags as c_int & P9_O_DIRECTORY != 0 {
        let dirp = unsafe { opendir((*h).path) };
        if dirp.is_null() {
            return -errno_to_p9(errno());
        }
        unsafe {
            (*h).is_opened = true;
            (*h).is_dir = true;
            (*h).dirp = dirp;
        }
    } else {
        let fd = unsafe { open((*h).path, (p9_flags_to_host(flags as c_int) & !O_CREAT) | O_NOFOLLOW) };
        if fd < 0 {
            return -errno_to_p9(errno());
        }
        unsafe {
            (*h).is_opened = true;
            (*h).is_dir = false;
            (*h).fd = fd;
        }
    }
    0
}
