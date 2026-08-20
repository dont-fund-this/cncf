use core::ffi::{c_char, CStr};

pub fn less(sid: *const c_char) {
    let sid = unsafe { CStr::from_ptr(sid) }.to_bytes();
    super::defs::mutable().retain(|def| {
        unsafe { CStr::from_ptr(def.sid) }.to_bytes() != sid
    });
}
