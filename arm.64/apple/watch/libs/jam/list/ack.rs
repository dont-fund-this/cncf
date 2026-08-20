use alloc::string::String;
use core::ffi::{c_char, CStr};

use crate::libs::load::libs;
use crate::reply::esc;
use crate::reply::text;

pub fn ack(options: *const c_char) {
    let into = match text(options, "into") {
        Some(into) => into,
        None => return,
    };
    let mut json = String::new();
    json.push_str("{\"ok\": true, \"libs\": [");
    let mut first = true;
    for abi in libs() {
        let info = unsafe { (abi.report)() };
        if info.sig.is_null() || info.tag.is_null() {
            continue;
        }
        if !first {
            json.push_str(", ");
        }
        first = false;
        json.push_str("{\"sig\": \"");
        esc(unsafe { CStr::from_ptr(info.sig) }.to_bytes(), &mut json);
        json.push_str("\", \"tag\": \"");
        esc(unsafe { CStr::from_ptr(info.tag) }.to_bytes(), &mut json);
        json.push_str("\"}");
    }
    json.push_str("]}");
    json.push('\0');
    crate::host::invoke(
        into.as_ptr() as *const c_char,
        json.as_ptr() as *const c_char,
        c"{\"strict\": \"once\"}".as_ptr(),
    );
}
