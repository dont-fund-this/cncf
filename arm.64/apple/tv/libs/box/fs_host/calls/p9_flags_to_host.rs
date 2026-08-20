use core::ffi::c_int;

use super::abi::*;
use super::super::dev::*;

pub fn p9_flags_to_host(flags: c_int) -> c_int {
    let mut ret = flags & P9_O_NOACCESS;
    if flags & P9_O_CREAT != 0 {
        ret |= O_CREAT;
    }
    if flags & P9_O_EXCL != 0 {
        ret |= O_EXCL;
    }
    if flags & P9_O_TRUNC != 0 {
        ret |= O_TRUNC;
    }
    if flags & P9_O_APPEND != 0 {
        ret |= O_APPEND;
    }
    if flags & P9_O_NONBLOCK != 0 {
        ret |= O_NONBLOCK;
    }
    if flags & P9_O_DSYNC != 0 {
        ret |= O_DSYNC;
    }
    if flags & P9_O_NOFOLLOW != 0 {
        ret |= O_NOFOLLOW;
    }
    if flags & P9_O_SYNC != 0 {
        ret |= O_SYNC;
    }
    ret
}
