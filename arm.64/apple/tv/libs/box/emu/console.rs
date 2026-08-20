use core::ffi::{c_int, c_void};

extern "C" {
    pub fn virtio_console_get_write_len(s: *mut c_void) -> c_int;
    pub fn virtio_console_write_data(s: *mut c_void, buf: *const u8, buf_len: c_int) -> c_int;
}
