use alloc::string::String;
use core::ffi::c_char;

use super::text::text;

pub fn err(options: *const c_char, msg: &str) {
    let into = match text(options, "into") {
        Some(into) => into,
        None => return,
    };
    let mut json = String::from("{\"ok\": false, \"error\": \"");
    json.push_str(msg);
    json.push_str("\"}");
    json.push('\0');
    crate::abi::invoke(
        into.as_ptr() as *const c_char,
        json.as_ptr() as *const c_char,
        c"{\"strict\":\"once\"}".as_ptr(),
    );
}
