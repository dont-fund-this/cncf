use alloc::string::String;

pub fn esc(bytes: &[u8], out: &mut String) {
    for &b in bytes {
        match b {
            b'"' => out.push_str("\\\""),
            b'\\' => out.push_str("\\\\"),
            b'\n' => out.push_str("\\n"),
            b'\r' => out.push_str("\\r"),
            b'\t' => out.push_str("\\t"),
            0x20..=0x7e => out.push(b as char),
            _ => {
                out.push_str("\\u00");
                out.push(hex(b >> 4));
                out.push(hex(b & 0x0f));
            }
        }
    }
}

fn hex(n: u8) -> char {
    match n {
        0..=9 => (b'0' + n) as char,
        _ => (b'a' + (n - 10)) as char,
    }
}
