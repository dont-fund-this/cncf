use crate::r#type::Options;
use core::ffi::CStr;

pub fn many(options: Options) -> bool {
    if options.is_null() {
        return false;
    }
    let opt = unsafe { CStr::from_ptr(options).to_bytes() };
    opt == b"many"
}
