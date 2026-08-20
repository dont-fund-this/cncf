use crate::r#type::{Address, Options};
use core::ffi::CStr;

pub fn get_into(options: Options) -> Option<Address> {
    if options.is_null() {
        return None;
    }
    let opt = unsafe { CStr::from_ptr(options).to_bytes() };
    if let Some(pos) = opt.windows(5).position(|w| w == b"into:") {
        return Some(unsafe { options.add(pos + 5) });
    }
    if opt.starts_with(b"{") || opt.starts_with(b"[") {
        return None;
    }
    if !opt.is_empty() && opt != b"once" && opt != b"many" && opt != b"none" {
        return Some(options);
    }
    None
}

pub fn has_verb(options: Options, verb: &[u8]) -> bool {
    if options.is_null() {
        return verb == b"GET";
    }
    let opt = unsafe { CStr::from_ptr(options).to_bytes() };
    if let Some(pos) = opt.windows(5).position(|w| w == b"verb:") {
        let rest = &opt[pos + 5..];
        let vlen = rest.iter().position(|&b| b == b',' || b == 0).unwrap_or(rest.len());
        return &rest[..vlen] == verb;
    }
    verb == b"GET"
}
