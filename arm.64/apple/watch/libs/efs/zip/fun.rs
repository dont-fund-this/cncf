use core::ffi::c_char;

use super::fit::fit;
use super::text::text;
use super::resolve::resolve;
use super::frame::frame;
use crate::save::save;

pub unsafe fn fun(
    address: *const c_char,
    payload: *const c_char,
    options: *const c_char,
) {
    let fits = unsafe { fit(address, payload, options) };
    if !fits {
        return;
    }
    let path = match text(payload, "path") {
        Some(p) => p,
        None => return,
    };
    let dest = resolve(&path);
    if dest.is_empty() {
        return;
    }
    save(&dest, &frame());
}
