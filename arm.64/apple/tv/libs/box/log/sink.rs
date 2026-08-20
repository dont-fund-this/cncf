use core::ffi::{c_int, c_void};

pub unsafe extern "C" fn sink(_opaque: *mut c_void, buf: *const u8, len: c_int) {
    if buf.is_null() || len <= 0 {
        return;
    }
    let bytes = unsafe { core::slice::from_raw_parts(buf, len as usize) };
    let out = unsafe { &mut *core::ptr::addr_of_mut!(super::hold::OUT) };
    out.extend_from_slice(bytes);
}
