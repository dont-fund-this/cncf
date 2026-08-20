use alloc::vec::Vec;

use crate::emu::CharacterDevice;

pub static mut OUT: Vec<u8> = Vec::new();

pub static mut CHAR: CharacterDevice = CharacterDevice {
    opaque: core::ptr::null_mut(),
    write_data: None,
    read_data: None,
};
