use alloc::string::String;
use core::ffi::c_char;

use super::esc::esc;
use super::text::text;

pub fn ack(options: *const c_char, files: &[&str]) {
    let into = match text(options, "into") {
        Some(into) => into,
        None => return,
    };
    let mut json = String::from("{\"files\": [");
    let mut first = true;
    for f in files {
        if !first {
            json.push_str(", ");
        }
        first = false;
        json.push('"');
        esc(f.as_bytes(), &mut json);
        json.push('"');
    }
    json.push_str("]}");
    json.push('\0');
    crate::abi::invoke(
        into.as_ptr() as *const c_char,
        json.as_ptr() as *const c_char,
        c"{\"strict\":\"once\"}".as_ptr(),
    );
}
