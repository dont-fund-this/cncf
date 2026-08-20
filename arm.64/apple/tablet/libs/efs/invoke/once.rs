use core::ffi::c_int;
use core::ffi::c_char;

use crate::with::defs;

pub unsafe extern "C" fn once(
    address: *const c_char,
    payload: *const c_char,
    options: *const c_char,
) -> c_int {
    for def in defs() {
        if unsafe { (def.fit)(address, payload, options) } {
            unsafe { (def.fun)(address, payload, options) };
            return 1;
        }
    }
    0
}
