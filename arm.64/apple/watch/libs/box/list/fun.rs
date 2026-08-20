use core::ffi::c_char;

use super::fit::fit;
use super::dir::dir;
use super::text::text;

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
        Some(path) => path,
        None => return,
    };
    dir(path.as_ptr() as *const c_char);
}
