use core::ffi::{c_int, c_void};

#[repr(C)]
pub struct PhysMemoryRange {
    pub map: *mut c_void,
    pub addr: u64,
    pub org_size: u64,
    pub size: u64,
    pub is_ram: c_int,
    pub devram_flags: c_int,
    pub phys_mem: *mut u8,
}

extern "C" {
    pub fn register_ram_entry(
        s: *mut c_void,
        addr: u64,
        size: u64,
        devram_flags: c_int,
    ) -> *mut PhysMemoryRange;
}
