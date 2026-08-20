use alloc::vec::Vec;

use crate::emu::BlockDevice;

pub static mut BACK: Option<Vec<u8>> = None;

pub static mut DEV: BlockDevice = BlockDevice {
    get_sector_count: None,
    read_async: None,
    write_async: None,
    opaque: core::ptr::null_mut(),
};
