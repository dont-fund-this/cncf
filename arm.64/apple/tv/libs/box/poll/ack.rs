use alloc::string::String;
use core::ffi::c_char;
use crate::reply::{esc, text};
use crate::log::drain;

pub fn ack(options: *const c_char, generation: u32) {
    let into = match text(options, "into") {
        Some(into) => into,
        None => return,
    };
    let out = drain();
    let mut json = String::new();
    json.push_str("{\"ok\": true, \"bell\": ");
    digits(generation, &mut json);
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

fn digits(mut n: u32, out: &mut String) {
    let mut buf = [0u8; 10];
    let mut i = buf.len();
    loop {
        i -= 1;
        buf[i] = b'0' + (n % 10) as u8;
        n /= 10;
        if n == 0 { break; }
    }
    out.push_str(core::str::from_utf8(&buf[i..]).unwrap_or("0"));
}
