use crate::emu::CharacterDevice;
use super::hold::{CHAR, OUT};

pub fn free() {
    unsafe {
        *core::ptr::addr_of_mut!(OUT) = alloc::vec::Vec::new();
        *core::ptr::addr_of_mut!(CHAR) = CharacterDevice {
            opaque: core::ptr::null_mut(),
            write_data: None,
            read_data: None,
        };
    }
}
