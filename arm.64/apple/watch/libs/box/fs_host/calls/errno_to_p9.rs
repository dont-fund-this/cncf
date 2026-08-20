use core::ffi::c_int;

use super::abi::*;
use super::super::dev::*;

pub fn errno_to_p9(err: c_int) -> c_int {
    if err == 0 {
        return 0;
    }
    match err {
        E_PERM => P9_EPERM,
        E_NOENT => P9_ENOENT,
        E_IO => P9_EIO,
        E_EXIST => P9_EEXIST,
        E_INVAL => P9_EINVAL,
        E_NOSPC => P9_ENOSPC,
        E_NOTEMPTY => P9_ENOTEMPTY,
        E_PROTO => P9_EPROTO,
        E_NOTSUP => P9_ENOTSUP,
        _ => P9_EINVAL,
    }
}
