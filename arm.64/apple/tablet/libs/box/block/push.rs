use core::ffi::{c_int, c_void};

use crate::emu::{BlockDevice, BlockDeviceCompletionFunc};

pub unsafe extern "C" fn push(
    _bs: *mut BlockDevice,
    sector: u64,
    buf: *const u8,
    n: c_int,
    _cb: Option<BlockDeviceCompletionFunc>,
    _opaque: *mut c_void,
) -> c_int {
    let back = match unsafe { &mut *core::ptr::addr_of_mut!(super::hold::BACK) } {
        Some(back) => back,
        None => return -1,
    };
    let size = back.len() as i64;
    let off = sector as i64 * 512;
    let len = n as i64 * 512;
    if off < 0 || len < 0 || off + len > size {
        return -1;
    }
    unsafe {
        core::ptr::copy_nonoverlapping(buf, back.as_mut_ptr().add(off as usize), len as usize);
    }
    0
}
