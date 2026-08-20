use core::ffi::c_char;

use super::ack::ack;
use super::fit::fit;
use super::run::run;

pub unsafe fn fun(
    address: *const c_char,
    payload: *const c_char,
    options: *const c_char,
) {
    let fits = unsafe { fit(address, payload, options) };
    if !fits {
        return;
    }
    ack(options, run());
}
