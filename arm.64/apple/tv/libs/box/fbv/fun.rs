use core::ffi::c_char;

use crate::fb::{H, PTR, W};
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
    let ptr = unsafe { *core::ptr::addr_of!(PTR) };
    let w = unsafe { *core::ptr::addr_of!(W) };
    let h = unsafe { *core::ptr::addr_of!(H) };
    ack(options, ptr as usize, w as usize, h as usize);
}
