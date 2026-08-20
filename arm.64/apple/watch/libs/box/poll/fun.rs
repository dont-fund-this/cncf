use core::ffi::c_char;

use crate::run::tick;
use crate::vm::get;
use super::ack::ack;
use super::fit::fit;

pub unsafe fn fun(
    address: *const c_char,
    payload: *const c_char,
    options: *const c_char,
) {
    let fits = unsafe { fit(address, payload, options) };
    if !fits {
        return;
    }
    if get().is_null() {
        return;
    }
    let mut n = 0;
    while n < 131072 {
        tick();
        n += 1;
    }
    let bell_val = crate::bell::changed().unwrap_or(0);
    ack(options, bell_val);
}
