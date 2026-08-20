use core::ffi::c_void;

use super::abi::*;
use super::close::boxfs_close;
use super::super::dev::*;
use super::super::file::*;

pub unsafe extern "C" fn boxfs_delete(fs1: *mut FSDevice, f: *mut FSFile) {
    let h = f as *mut Handle;
    if unsafe { (*h).is_opened } {
        unsafe { boxfs_close(fs1, f) };
    }
    free_c(unsafe { (*h).path });
    unsafe { free(h as *mut c_void) };
}
