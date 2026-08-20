use alloc::vec::Vec;

use super::hold::OUT;

pub fn drain() -> Vec<u8> {
    unsafe {
        let out = &mut *core::ptr::addr_of_mut!(OUT);
        core::mem::take(out)
    }
}
