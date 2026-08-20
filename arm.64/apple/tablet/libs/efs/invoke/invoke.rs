use core::ffi::{c_char, c_int};

use super::strict::{strict, Strict};
use super::none::none;
use super::once::once;
use super::many::many;

#[no_mangle]
pub unsafe extern "C" fn invoke(
    address: *const c_char,
    payload: *const c_char,
    options: *const c_char,
) -> c_int {
    match strict(options) {
        Some(Strict::None) => unsafe { none(address, payload, options) },
        Some(Strict::Once) => unsafe { once(address, payload, options) },
        Some(Strict::Many) => unsafe { many(address, payload, options) },
        None => 0,
    }
}
