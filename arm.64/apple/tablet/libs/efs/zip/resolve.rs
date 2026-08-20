use alloc::string::String;
use core::ffi::{c_char, CStr};

extern "C" {
    fn getenv(name: *const c_char) -> *const c_char;
}

pub fn resolve(path: &str) -> String {
    let path = path.trim_end_matches('\0');
    match path.strip_prefix('$') {
        Some(name) => {
            let mut key = String::from(name);
            key.push('\0');
            let v = unsafe { getenv(key.as_ptr() as *const c_char) };
            if v.is_null() {
                String::new()
            } else {
                String::from(unsafe { CStr::from_ptr(v) }.to_str().unwrap_or(""))
            }
        }
        None => String::from(path),
    }
}
