use core::ffi::{c_char, c_int};

use super::text::text;

pub fn fbdim(payload: *const c_char) -> (c_int, c_int) {
    let s = match text(payload, "fb") {
        Some(s) => s,
        None => return (0, 0),
    };
    let bytes = s.as_bytes();
    let mut w: c_int = 0;
    let mut h: c_int = 0;
    let mut seen_x = false;
    let mut any_w = false;
    let mut any_h = false;
    for &b in bytes {
        if b == b'x' || b == b'X' {
            if seen_x {
                return (0, 0);
            }
            seen_x = true;
        } else if b.is_ascii_digit() {
            let d = (b - b'0') as c_int;
            if seen_x {
                h = h * 10 + d;
                any_h = true;
            } else {
                w = w * 10 + d;
                any_w = true;
            }
        } else if b == 0 {
            break;
        } else {
            return (0, 0);
        }
    }
    if !seen_x || !any_w || !any_h {
        return (0, 0);
    }
    (w, h)
}
