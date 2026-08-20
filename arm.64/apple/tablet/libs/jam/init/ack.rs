use alloc::string::String;
use core::ffi::c_char;

use crate::reply::text;

pub fn ack(options: *const c_char, loaded: Option<usize>) {
    let into = match text(options, "into") {
        Some(into) => into,
        None => return,
    };
    let mut json = String::new();
    match loaded {
        None => json.push_str("{\"ok\": false}"),
        Some(n) => {
            json.push_str("{\"ok\": true, \"loaded\": ");
            digits(n, &mut json);
            json.push_str("}");
        }
    }
    json.push('\0');
    crate::host::invoke(
        into.as_ptr() as *const c_char,
        json.as_ptr() as *const c_char,
        c"{\"strict\": \"once\"}".as_ptr(),
    );
}

fn digits(n: usize, out: &mut String) {
    if n == 0 {
        out.push('0');
        return;
    }
    let mut buf = [0u8; 20];
    let mut i = buf.len();
    let mut v = n;
    while v > 0 {
        i -= 1;
        buf[i] = b'0' + (v % 10) as u8;
        v /= 10;
    }
    out.push_str(core::str::from_utf8(&buf[i..]).unwrap_or("0"));
}
