use crate::r#type::Options;
use core::ffi::CStr;

pub fn once(options: Options) -> bool {
    if options.is_null() {
        return false;
    }
    let opt = unsafe { CStr::from_ptr(options).to_bytes() };
    opt == b"once"
        || opt.starts_with(b"into:")
        || contains(opt, b"\"once\":true")
        || contains(opt, b"\"once\": true")
}

fn contains(haystack: &[u8], needle: &[u8]) -> bool {
    if needle.len() > haystack.len() {
        return false;
    }
    let mut i = 0;
    while i + needle.len() <= haystack.len() {
        let mut matched = true;
        let mut j = 0;
        while j < needle.len() {
            if haystack[i + j] != needle[j] {
                matched = false;
                break;
            }
            j += 1;
        }
        if matched {
            return true;
        }
        i += 1;
    }
    false
}
