use std::ffi::{c_char, c_int};

use crate::defs::defs;
use crate::strict::{strict, Strict};

#[no_mangle]
pub unsafe extern "C" fn invoke(
    address: *const c_char,
    payload: *const c_char,
    options: *const c_char,
) -> c_int {
    match strict(options) {
        Some(Strict::None) => defs().iter().filter(|def| unsafe {
            (def.fit)(address, payload, options)
        }).count() as c_int,
        Some(Strict::Once) => {
            for def in defs() {
                if unsafe { (def.fit)(address, payload, options) } {
                    unsafe { (def.fun)(address, payload, options) };
                    return 1;
                }
            }
            0
        }
        Some(Strict::Many) => {
            let mut count = 0;
            for def in defs() {
                if unsafe { (def.fit)(address, payload, options) } {
                    unsafe { (def.fun)(address, payload, options) };
                    count += 1;
                }
            }
            count
        }
        None => 0,
    }
}
