use core::ffi::c_char;
use alloc::string::String;

use super::fit::fit;
use super::text::text;
use crate::unb64::unb64;

pub unsafe fn fun(
    address: *const c_char,
    payload: *const c_char,
    options: *const c_char,
) {
    let fits = unsafe { fit(address, payload, options) };
    if !fits {
        return;
    }
    let coded = match text(payload, "text") {
        Some(t) => t,
        None => return,
    };
    let bytes = unb64(coded.trim_end_matches('\0'));
    let mut held = match core::str::from_utf8(&bytes) {
        Ok(s) => String::from(s),
        Err(_) => return,
    };
    held.push('\0');
    crate::hold::set(held);
}
