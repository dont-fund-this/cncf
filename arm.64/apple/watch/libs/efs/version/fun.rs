use core::ffi::c_int;
use core::ffi::c_char;

use super::fit::fit;

extern "C" {
    fn puts(s: *const c_char) -> c_int;
}

pub unsafe fn fun(
    address: *const c_char,
    payload: *const c_char,
    options: *const c_char,
) {
    let fits = unsafe { fit(address, payload, options) };
    if !fits {
        return;
    }
    unsafe {
        puts(address);
        puts(payload);
        puts(options);
    }
}
