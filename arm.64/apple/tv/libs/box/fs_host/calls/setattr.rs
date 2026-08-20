use core::ffi::c_int;

use super::abi::*;
use super::errno::errno;
use super::errno_to_p9::errno_to_p9;
use super::is_ro::is_ro;
use super::super::dev::*;
use super::super::file::*;

pub unsafe extern "C" fn boxfs_setattr(
    fs1: *mut FSDevice,
    f: *mut FSFile,
    mask: u32,
    mode: u32,
    uid: u32,
    gid: u32,
    size: u64,
    atime_sec: u64,
    atime_nsec: u64,
    mtime_sec: u64,
    mtime_nsec: u64,
) -> c_int {
    if is_ro(fs1) {
        return -P9_EPERM;
    }
    let h = f as *mut Handle;
    let path = unsafe { (*h).path };
    let mut ctime_updated = false;
    if mask & (P9_SETATTR_UID | P9_SETATTR_GID) != 0 {
        let u = if mask & P9_SETATTR_UID != 0 { uid } else { u32::MAX };
        let g = if mask & P9_SETATTR_GID != 0 { gid } else { u32::MAX };
        if unsafe { lchown(path, u, g) } < 0 {
            return -errno_to_p9(errno());
        }
        ctime_updated = true;
    }
    if mask & P9_SETATTR_MODE != 0 {
        if unsafe { chmod(path, mode as u16) } < 0 {
            return -errno_to_p9(errno());
        }
        ctime_updated = true;
    }
    if mask & P9_SETATTR_SIZE != 0 {
        if unsafe { truncate(path, size as i64) } < 0 {
            return -errno_to_p9(errno());
        }
        ctime_updated = true;
    }
    if mask & (P9_SETATTR_ATIME | P9_SETATTR_MTIME) != 0 {
        let mut ts: [Timespec; 2] = unsafe { core::mem::zeroed() };
        if mask & P9_SETATTR_ATIME != 0 {
            if mask & P9_SETATTR_ATIME_SET != 0 {
                ts[0].tv_sec = atime_sec as i64;
                ts[0].tv_nsec = atime_nsec as i64;
            } else {
                ts[0].tv_sec = 0;
                ts[0].tv_nsec = UTIME_NOW;
            }
        } else {
            ts[0].tv_sec = 0;
            ts[0].tv_nsec = UTIME_OMIT;
        }
        if mask & P9_SETATTR_MTIME != 0 {
            if mask & P9_SETATTR_MTIME_SET != 0 {
                ts[1].tv_sec = mtime_sec as i64;
                ts[1].tv_nsec = mtime_nsec as i64;
            } else {
                ts[1].tv_sec = 0;
                ts[1].tv_nsec = UTIME_NOW;
            }
        } else {
            ts[1].tv_sec = 0;
            ts[1].tv_nsec = UTIME_OMIT;
        }
        if unsafe { utimensat(AT_FDCWD, path, ts.as_ptr(), AT_SYMLINK_NOFOLLOW) } < 0 {
            return -errno_to_p9(errno());
        }
        ctime_updated = true;
    }
    if mask & P9_SETATTR_CTIME != 0 && !ctime_updated {
        if unsafe { lchown(path, u32::MAX, u32::MAX) } < 0 {
            return -errno_to_p9(errno());
        }
    }
    0
}
