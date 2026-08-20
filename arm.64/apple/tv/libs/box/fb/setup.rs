use core::ffi::{c_int, c_void};

use crate::emu::{register_ram_entry, VirtMachine};
use super::hold::{PR, PTR, SIZE, W, H};
use super::ADDR;

const PAGE: u64 = 4096;

extern "C" {
    fn posix_memalign(memptr: *mut *mut c_void, alignment: usize, size: usize) -> c_int;
}

pub fn setup(w: c_int, h: c_int, vm: *mut VirtMachine) -> bool {
    if w <= 0 || h <= 0 || vm.is_null() {
        return false;
    }
    let raw = (w as u64) * (h as u64) * 2;
    let size = (raw + PAGE - 1) & !(PAGE - 1);
    
    let mut ptr: *mut c_void = core::ptr::null_mut();
    let res = unsafe { posix_memalign(&mut ptr, 4096, size as usize) };
    if res != 0 || ptr.is_null() {
        return false;
    }
    
    let con = unsafe { (*vm).console_dev };
    if con.is_null() {
        unsafe {
            extern "C" { fn free(ptr: *mut c_void); }
            free(ptr);
        }
        return false;
    }
    let map = unsafe { *(con as *mut *mut c_void) };
    let pr = unsafe { register_ram_entry(map, ADDR, size, 0) };
    if pr.is_null() {
        unsafe {
            extern "C" { fn free(ptr: *mut c_void); }
            free(ptr);
        }
        return false;
    }
    unsafe { (*pr).phys_mem = ptr as *mut u8 };
    unsafe {
        *core::ptr::addr_of_mut!(PTR) = ptr;
        *core::ptr::addr_of_mut!(PR) = pr;
        *core::ptr::addr_of_mut!(SIZE) = size;
        *core::ptr::addr_of_mut!(W) = w;
        *core::ptr::addr_of_mut!(H) = h;
    }
    true
}
