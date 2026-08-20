use core::ffi::{c_char, CStr};

pub fn num(payload: *const c_char, key: &str, def: u64) -> u64 {
    let json = unsafe { CStr::from_ptr(payload) };
    let src = json.to_bytes();
    let pat = key.as_bytes();
    let mut i = 0;
    while i + pat.len() < src.len() {
        if src[i] == b'"' && matches(&src[i + 1..], pat) {
            let mut j = i + 1 + pat.len();
            while j < src.len() && src[j] != b'"' {
                j += 1;
            }
            j += 1;
            while j < src.len() && (src[j] == b' ' || src[j] == b':' || src[j] == b'\t') {
                j += 1;
            }
            let mut val: u64 = 0;
            let mut seen = false;
            while j < src.len() && src[j].is_ascii_digit() {
                val = val * 10 + (src[j] - b'0') as u64;
                seen = true;
                j += 1;
            }
            if seen {
                return val;
            }
            return def;
        }
        i += 1;
    }
    def
}

fn matches(src: &[u8], pat: &[u8]) -> bool {
    if src.len() < pat.len() + 1 {
        return false;
    }
    if &src[..pat.len()] != pat {
        return false;
    }
    src[pat.len()] == b'"'
}
