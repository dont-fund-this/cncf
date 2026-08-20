use core::ffi::c_char;

use super::fit::fit;
use super::text::text;
use super::err::err;
use super::ack::ack;
use crate::pool::{entries, blob_orig};

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
            err(options, "empty path");
            return;
        }
    };
    let path = path.trim_end_matches('\0');
    if path.is_empty() {
        err(options, "empty path");
        return;
    }
    for e in entries() {
        if e.path.as_str() == path {
            ack(options, blob_orig(e.blob) as usize);
            return;
        }
    }
    err(options, "not found");
}
