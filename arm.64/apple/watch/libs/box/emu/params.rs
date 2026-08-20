use core::ffi::{c_char, c_int, c_void};

use super::entry::{VmDriveEntry, VmEthEntry, VmFileEntry, VmFsEntry};

#[repr(C)]
pub struct VirtMachineParams {
    pub cfg_filename: *mut c_char,
    pub vmc: *const c_void,
    pub machine_name: *mut c_char,
    pub ram_size: u64,
    pub rtc_real_time: c_int,
    pub rtc_local_time: c_int,
    pub display_device: *mut c_char,
    pub width: c_int,
    pub height: c_int,
    pub console: *mut c_void,
    pub tab_drive: [VmDriveEntry; 4],
    pub drive_count: c_int,
    pub tab_fs: [VmFsEntry; 4],
    pub fs_count: c_int,
    pub tab_eth: [VmEthEntry; 1],
    pub eth_count: c_int,
    pub cmdline: *mut c_char,
    pub accel_enable: c_int,
    pub input_device: *mut c_char,
    pub files: [VmFileEntry; 4],
}
