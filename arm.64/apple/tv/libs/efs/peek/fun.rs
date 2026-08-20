use core::ffi::c_char;

use super::fit::fit;
use super::text::text;
use super::count::count;
use super::err::err;
use super::ack::ack;
use crate::pool::data;
use crate::b64::b64;

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
        None => {
            err(options, "missing path");
            return;
        }
    };
    let path = path.trim_end_matches('\0');
    if path.is_empty() {
        err(options, "missing path");
        return;
    }
    let want = count(payload, "count", 512);
    let want = if want == 0 { 512 } else { want as usize };
    let bytes = match data(path) {
        Some(b) => b,
        None => {
            err(options, "not found");
            return;
        }
    };
    let n = if want < bytes.len() { want } else { bytes.len() };
    ack(options, n, &b64(&bytes[..n]));
}
