use alloc::vec::Vec;
use core::ffi::{c_char, c_int, c_void, CStr};

use crate::type_::Def;

pub fn out() -> Def {
    Def {
        sid: c"tui.out".as_ptr(),
        tag: c"out".as_ptr(),
        fun,
        fit,
    }
}

unsafe fn fit(address: *const c_char, _payload: *const c_char, _options: *const c_char) -> bool {
    let a = unsafe { CStr::from_ptr(address) };
    a == c"tui.out" || a == c"out"
}

unsafe fn fun(address: *const c_char, payload: *const c_char, options: *const c_char) {
    if !unsafe { fit(address, payload, options) } {
        return;
    }
    let s = unsafe { CStr::from_ptr(payload) }.to_bytes();
    let key = b"\"out\"";
    let mut i = 0usize;
    while i + key.len() <= s.len() && &s[i..i + key.len()] != key {
        i += 1;
    }
    if i + key.len() > s.len() {
        return;
    }
    i += key.len();
    while i < s.len() && s[i] != b'"' {
        i += 1;
    }
    if i >= s.len() {
        return;
    }
    i += 1;
    let mut buf: Vec<u8> = Vec::new();
    while i < s.len() {
        let b = s[i];
        if b == b'"' {
            break;
        }
        if b == b'\\' {
            i += 1;
            if i >= s.len() {
                break;
            }
            match s[i] {
                b'n' => buf.push(0x0a),
                b'r' => buf.push(0x0d),
                b't' => buf.push(0x09),
                b'"' => buf.push(b'"'),
                b'\\' => buf.push(b'\\'),
                b'u' => {
                    if i + 4 < s.len() {
                        buf.push((hexv(s[i + 3]) << 4) | hexv(s[i + 4]));
                        i += 4;
                    }
                }
                other => buf.push(other),
            }
        } else {
            buf.push(b);
        }
        i += 1;
    }
    if !buf.is_empty() {
        extern "C" {
            fn write(fd: c_int, buf: *const c_void, n: usize) -> isize;
        }
        unsafe { write(1, buf.as_ptr() as *const c_void, buf.len()) };
    }
}

fn hexv(c: u8) -> u8 {
    match c {
        b'0'..=b'9' => c - b'0',
        b'a'..=b'f' => c - b'a' + 10,
        b'A'..=b'F' => c - b'A' + 10,
        _ => 0,
    }
}
