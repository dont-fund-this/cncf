use std::ffi::{c_char, CStr};

pub enum Strict {
    None,
    Once,
    Many,
}

pub fn strict(options: *const c_char) -> Option<Strict> {
    if options.is_null() {
        return None;
    }
    let src = unsafe { CStr::from_ptr(options) }.to_bytes();
    let key = b"\"strict\"";
    let start = src.windows(key.len()).position(|part| part == key)? + key.len();
    let mut tail = trim(&src[start..]);
    tail = trim(tail.strip_prefix(b":")?);
    let value = tail.strip_prefix(b"\"")?;
    let end = value.iter().position(|byte| *byte == b'\"')?;
    match &value[..end] {
        b"none" => Some(Strict::None),
        b"once" => Some(Strict::Once),
        b"many" => Some(Strict::Many),
        _ => None,
    }
}

fn trim(mut src: &[u8]) -> &[u8] {
    while matches!(src.first(), Some(b' ' | b'\t' | b'\n' | b'\r')) {
        src = &src[1..];
    }
    src
}
