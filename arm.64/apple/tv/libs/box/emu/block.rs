use core::ffi::c_void;

pub type BlockDeviceCompletionFunc = unsafe extern "C" fn(*mut c_void, c_int);

use core::ffi::c_int;

#[repr(C)]
pub struct BlockDevice {
    pub get_sector_count: Option<unsafe extern "C" fn(*mut BlockDevice) -> i64>,
    pub read_async: Option<
        unsafe extern "C" fn(
            *mut BlockDevice,
            u64,
            *mut u8,
            c_int,
            Option<BlockDeviceCompletionFunc>,
            *mut c_void,
        ) -> c_int,
    >,
    pub write_async: Option<
        unsafe extern "C" fn(
            *mut BlockDevice,
            u64,
            *const u8,
            c_int,
            Option<BlockDeviceCompletionFunc>,
            *mut c_void,
        ) -> c_int,
    >,
    pub opaque: *mut c_void,
}
