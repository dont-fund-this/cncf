use core::ffi::{c_int, c_void};

pub unsafe extern "C" fn void(_opaque: *mut c_void, _buf: *mut u8, _len: c_int) -> c_int {
    0
}
