use core::ffi::c_char;

use crate::run::end;
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
    end();
    emit(options, false);
}
