use core::ffi::{c_char, CStr};

use super::tag::tag;

pub unsafe fn fit_tag(address: *const c_char) -> bool {
    let addr = match unsafe { CStr::from_ptr(address) }.to_str() {
        Ok(s) => s,
        Err(_) => return false,
    };
    let tags = match tag().to_str() {
        Ok(s) => s,
        Err(_) => return false,
    };
    for token in tags.split(',') {
        if token.trim() == addr {
            return true;
        }
    }
    false
}
