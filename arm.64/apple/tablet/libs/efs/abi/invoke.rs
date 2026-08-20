pub fn invoke(
    address: *const core::ffi::c_char,
    payload: *const core::ffi::c_char,
    options: *const core::ffi::c_char,
) -> core::ffi::c_int {
    match unsafe { super::ABI_INVOKE } {
        Some(cb) => unsafe { cb(address, payload, options) },
        None => 0,
    }
}
