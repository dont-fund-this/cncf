use core::ffi::{c_char, CStr};

use super::sid::sid;

pub unsafe fn fit_sid(address: *const c_char) -> bool {
    unsafe { CStr::from_ptr(address) == sid() }
}
