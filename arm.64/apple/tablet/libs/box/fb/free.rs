use core::ffi::{c_int, c_void};

use crate::emu::PhysMemoryRange;
use super::hold::{H, PR, PTR, SIZE, W};

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
        *core::ptr::addr_of_mut!(W) = 0 as c_int;
        *core::ptr::addr_of_mut!(H) = 0 as c_int;
    }
}
