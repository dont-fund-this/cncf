use core::ffi::c_char;

use super::fit::fit;
use super::tty::tty;
use super::open::open;

pub unsafe fn fun(
    address: *const c_char,
    payload: *const c_char,
    options: *const c_char,
) {
    let fits = unsafe { fit(address, payload, options) };
    if !fits {
        return;
    }
    if !tty() {
        return;
    }
    open();
}
