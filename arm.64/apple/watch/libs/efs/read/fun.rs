use core::ffi::c_char;
use alloc::string::String;

use super::fit::fit;
use super::text::text;
use crate::pool::data;
use crate::b64::b64;
use crate::num::num;

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
        Some(p) => p,
        None => return,
    };
    let into = match text(options, "into") {
        Some(i) => i,
        None => return,
    };
    let bytes = match data(path.trim_end_matches('\0')) {
        Some(b) => b,
        None => return,
    };
    let mut reply = String::from("{\"size\": ");
    reply.push_str(&num(bytes.len()));
    reply.push_str(", \"text\": \"");
    reply.push_str(&b64(&bytes));
    reply.push_str("\"}");
    reply.push('\0');
    crate::abi::invoke(
        into.as_ptr() as *const c_char,
        reply.as_ptr() as *const c_char,
        c"{\"strict\":\"once\"}".as_ptr(),
    );
}
