use core::ffi::{c_char, c_int, c_void};

#[repr(C)]
pub struct VmFileEntry {
    pub filename: *mut c_char,
    pub buf: *mut u8,
    pub len: c_int,
}

#[repr(C)]
pub struct VmDriveEntry {
    pub device: *mut c_char,
    pub filename: *mut c_char,
    pub block_dev: *mut c_void,
}

#[repr(C)]
pub struct VmFsEntry {
    pub device: *mut c_char,
    pub tag: *mut c_char,
    pub filename: *mut c_char,
    pub fs_dev: *mut c_void,
}

#[repr(C)]
pub struct VmEthEntry {
    pub driver: *mut c_char,
    pub ifname: *mut c_char,
    pub net: *mut c_void,
}
