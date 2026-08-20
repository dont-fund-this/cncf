use core::ffi::c_void;

use crate::emu::PhysMemoryRange;
use super::hold::{PR, PTR, SEEN, SIZE};

extern "C" {
    #[link_name = "free"]
    fn c_free(ptr: *mut c_void);
}

pub fn free(vm_exited: bool) {
    unsafe {
        let ptr = *core::ptr::addr_of!(PTR);
        if !ptr.is_null() && !vm_exited {
            c_free(ptr);
        }
        *core::ptr::addr_of_mut!(PTR) = core::ptr::null_mut();
        *core::ptr::addr_of_mut!(PR) = core::ptr::null_mut::<PhysMemoryRange>();
        *core::ptr::addr_of_mut!(SIZE) = 0;
        *core::ptr::addr_of_mut!(SEEN) = 0;
    }
}
