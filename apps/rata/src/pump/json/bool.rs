use crate::r#type::Payload;
use core::ffi::CStr;

pub fn is_valid_json(payload: Payload) -> bool {
    if payload.is_null() {
        return false;
    }
    let bytes = unsafe { CStr::from_ptr(payload).to_bytes() };
    if bytes.is_empty() {
        return false;
    }
    let first = bytes[0];
    let last = bytes[bytes.len() - 1];
    (first == b"{"[0] && last == b"}"[0]) || (first == b"["[0] && last == b"]"[0])
}
