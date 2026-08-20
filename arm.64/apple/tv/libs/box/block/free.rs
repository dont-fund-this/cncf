use super::hold::{BACK, DEV};

pub fn free() {
    unsafe {
        *core::ptr::addr_of_mut!(BACK) = None;
        let dev = &mut *core::ptr::addr_of_mut!(DEV);
        dev.get_sector_count = None;
        dev.read_async = None;
        dev.write_async = None;
        dev.opaque = core::ptr::null_mut();
    }
}
