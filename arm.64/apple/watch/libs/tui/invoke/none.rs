use core::ffi::c_int;
use core::ffi::c_char;

use crate::with::defs;

pub unsafe extern "C" fn none(
    address: *const c_char,
    payload: *const c_char,
    options: *const c_char,
) -> c_int {
    let mut count: c_int = 0;
    for def in defs() {
        if unsafe { (def.fit)(address, payload, options) } {
            count += 1;
        }
    }
    count
}
