use core::ffi::c_char;

use super::fit_sid::fit_sid;
use super::fit_tag::fit_tag;

pub unsafe fn fit(
    address: *const c_char,
    _payload: *const c_char,
    _options: *const c_char,
) -> bool {
    if unsafe { fit_sid(address) } {
        return true;
    }
    if unsafe { fit_tag(address) } {
        return true;
    }
    false
}
