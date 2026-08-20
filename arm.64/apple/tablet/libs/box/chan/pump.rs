use core::ffi::c_void;

use crate::emu::{virtio_console_get_write_len, virtio_console_write_data};
use crate::vm::get;
use super::hold::INPUT;

pub fn pump() {
    let vm = get();
    if vm.is_null() {
        return;
    }
    let con: *mut c_void = unsafe { (*vm).console_dev };
    if con.is_null() {
        return;
    }
    let input = unsafe { &mut *core::ptr::addr_of_mut!(INPUT) };
    if input.is_empty() {
        return;
    }
    let room = unsafe { virtio_console_get_write_len(con) };
    if room <= 0 {
        return;
    }
    let take = core::cmp::min(input.len(), room as usize);
    unsafe { virtio_console_write_data(con, input.as_ptr(), take as core::ffi::c_int) };
    input.drain(0..take);
}
