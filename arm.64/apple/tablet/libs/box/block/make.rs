use core::ffi::c_char;

use crate::emu::BlockDevice;
use super::hold::{BACK, DEV};
use super::read::read;
use super::count::count;
use super::pull::pull;
use super::push::push;

pub fn make(path: *const c_char) -> *mut BlockDevice {
    let back = match read(path) {
        Some(back) => back,
        None => return core::ptr::null_mut(),
    };
    unsafe {
        *core::ptr::addr_of_mut!(BACK) = Some(back);
        let dev = &mut *core::ptr::addr_of_mut!(DEV);
        dev.get_sector_count = Some(count);
        dev.read_async = Some(pull);
        dev.write_async = Some(push);
        dev.opaque = core::ptr::null_mut();
        core::ptr::addr_of_mut!(DEV)
    }
}
