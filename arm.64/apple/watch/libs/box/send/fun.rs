use core::ffi::c_char;

use crate::chan::push;
use crate::reply::text;
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
    let queued = match text(payload, "data") {
        Some(s) => {
            let bytes = s.as_bytes();
            let end = bytes.len().saturating_sub(1);
            push(&bytes[..end])
        }
        None => 0,
    };
    ack(options, queued);
}
