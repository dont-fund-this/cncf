use core::ffi::CStr;

pub fn sid() -> &'static CStr {
    c"tui.wind"
}
