use core::ffi::c_int;

use super::abi::*;
use super::errno::errno;
use super::errno_to_p9::errno_to_p9;
use super::is_ro::is_ro;
use super::super::dev::*;
use super::super::file::*;

pub unsafe extern "C" fn boxfs_lock(
    fs1: *mut FSDevice,
    f: *mut FSFile,
    lock: *const FSLock,
) -> c_int {
    let h = f as *mut Handle;
    if unsafe { !(*h).is_opened || (*h).is_dir } {
        return -P9_EPROTO;
    }
    if unsafe { (*lock).type_ } == P9_LOCK_TYPE_WRLCK && is_ro(fs1) {
        return -P9_EPERM;
    }
    let mut fl: Flock = unsafe { core::mem::zeroed() };
    fl.l_type = match unsafe { (*lock).type_ } {
        P9_LOCK_TYPE_RDLCK => F_RDLCK,
        P9_LOCK_TYPE_WRLCK => F_WRLCK,
        P9_LOCK_TYPE_UNLCK => F_UNLCK,
        _ => return -P9_EINVAL,
    };
    fl.l_whence = SEEK_SET as i16;
    fl.l_start = unsafe { (*lock).start } as i64;
    fl.l_len = unsafe { (*lock).length } as i64;
    let ret = unsafe { fcntl((*h).fd, F_SETLK, &fl) };
    if ret == 0 {
        return P9_LOCK_SUCCESS;
    }
    let e = errno();
    if e == E_AGAIN || e == E_ACCES {
        return P9_LOCK_BLOCKED;
    }
    -errno_to_p9(e)
}
