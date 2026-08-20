use core::ffi::c_char;

use crate::reply::emit;
use crate::run::boot;
use crate::start::cfg;
use crate::vm::get;
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
    if !get().is_null() {
        emit(options, true);
        return;
    }
    let c = cfg(payload);
    let ready = boot(
        &c.bios,
        &c.kernel,
        &c.drive,
        &c.cmdline,
        c.ram,
        &c.mounts,
        c.fb_w,
        c.fb_h,
    );
    emit(options, ready);
}
