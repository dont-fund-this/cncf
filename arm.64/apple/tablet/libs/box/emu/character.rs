use core::ffi::{c_int, c_void};

#[repr(C)]
pub struct CharacterDevice {
    pub opaque: *mut c_void,
    pub write_data: Option<unsafe extern "C" fn(*mut c_void, *const u8, c_int)>,
    pub read_data: Option<unsafe extern "C" fn(*mut c_void, *mut u8, c_int) -> c_int>,
}
