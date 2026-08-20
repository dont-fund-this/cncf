use alloc::string::String;
use core::ffi::c_char;

use super::text::text;
use crate::num::num;

pub fn ack(options: *const c_char, kind: &str, size: usize) {
    let into = match text(options, "into") {
        Some(into) => into,
        None => return,
    };
    let mut json = String::from("{\"ok\": true, \"kind\": \"");
    json.push_str(kind);
    json.push_str("\", \"size\": ");
    json.push_str(&num(size));
    json.push('}');
    json.push('\0');
    crate::abi::invoke(
        into.as_ptr() as *const c_char,
        json.as_ptr() as *const c_char,
        c"{\"strict\":\"once\"}".as_ptr(),
    );
}
