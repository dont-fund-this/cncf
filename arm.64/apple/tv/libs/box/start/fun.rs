use core::ffi::c_char;

use crate::run::tick;
use crate::vm::get;
use crate::reply::emit;
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
        emit(options, false);
        return;
    }
    let mut n = 0;
    while n < 64 {
        tick();
        n += 1;
    }
    emit(options, true);
}
