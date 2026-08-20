use alloc::string::String;
use alloc::vec::Vec;
use core::ffi::c_char;

use super::fit::fit;
use super::text::text;
use super::ack::ack;
use crate::pool::entries;

pub unsafe fn fun(
    address: *const c_char,
    payload: *const c_char,
    options: *const c_char,
) {
    let fits = unsafe { fit(address, payload, options) };
    if !fits {
        return;
    }
    let prefix = match text(payload, "path") {
        Some(p) => p,
        None => String::new(),
    };
    let prefix = prefix.trim_end_matches('\0');
    let mut files: Vec<&str> = Vec::new();
    for e in entries() {
        if prefix.is_empty() || e.path.as_str().starts_with(prefix) {
            files.push(e.path.as_str());
        }
    }
    ack(options, &files);
}
