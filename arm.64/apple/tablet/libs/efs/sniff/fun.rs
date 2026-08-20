use core::ffi::c_char;

use super::fit::fit;
use super::text::text;
use super::kind::kind;
use super::err::err;
use super::ack::ack;
use crate::pool::data;

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
    let bytes = match data(path) {
        Some(b) => b,
        None => {
            err(options, "not found");
            return;
        }
    };
    let n = if bytes.len() < 512 { bytes.len() } else { 512 };
    ack(options, kind(&bytes[..n]), bytes.len());
}
