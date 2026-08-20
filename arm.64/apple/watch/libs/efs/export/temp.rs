use alloc::string::String;
use core::ffi::{c_char, CStr};

extern "C" {
    fn getenv(name: *const c_char) -> *const c_char;
}

pub fn temp() -> String {
    let override_dir = unsafe { getenv(c"PAT_EFS_EXPORT_DIR".as_ptr()) };
    if !override_dir.is_null() {
        let path = unsafe { CStr::from_ptr(override_dir) }.to_str().unwrap_or("");
        let path = path.trim_end_matches('/');
        if !path.is_empty() {
            return String::from(path);
        }
    }
    let v = unsafe { getenv(c"TMPDIR".as_ptr()) };
    if v.is_null() {
        return String::from("/tmp");
    }
    let s = unsafe { CStr::from_ptr(v) }.to_str().unwrap_or("");
    let s = s.trim_end_matches('/');
    if s.is_empty() {
        return String::from("/tmp");
    }
    String::from(s)
}
