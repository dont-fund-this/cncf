use crate::r#type::Def;
use core::ffi::{c_char, c_void};

extern "C" {
    fn dlsym(handle: *mut c_void, symbol: *const c_char) -> *mut c_void;
}

pub unsafe fn bind_symbol(handle: *mut c_void, name: *const c_char) -> *mut Def {
    if handle.is_null() || name.is_null() {
        return core::ptr::null_mut();
    }
    dlsym(handle, name).cast()
}
