use alloc::string::String;
use core::ffi::c_char;

use crate::log::drain;
use super::esc::esc;
use super::text::text;

pub fn emit(options: *const c_char, running: bool) {
    let into = match text(options, "into") {
        Some(into) => into,
        None => return,
    };
    let out = drain();
    let mut json = String::new();
    json.push_str("{\"ok\": true, \"running\": ");
    json.push_str(if running { "true" } else { "false" });
    json.push_str(", \"out\": \"");
    esc(&out, &mut json);
    json.push_str("\"}");
    json.push('\0');
    crate::abi::invoke(
        into.as_ptr() as *const c_char,
        json.as_ptr() as *const c_char,
        c"{\"strict\": \"once\"}".as_ptr(),
    );
}
