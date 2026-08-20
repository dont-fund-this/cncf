use core::ffi::CStr;

pub fn sid() -> &'static CStr {
    c"efs.load"
}
