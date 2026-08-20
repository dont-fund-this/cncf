use super::abi::*;
use super::super::dev::*;
use super::super::file::*;

pub unsafe extern "C" fn boxfs_close(_fs1: *mut FSDevice, f: *mut FSFile) {
    let h = f as *mut Handle;
    if unsafe { !(*h).is_opened } {
        return;
    }
    if unsafe { (*h).is_dir } {
        unsafe { closedir((*h).dirp) };
    } else {
        unsafe { close((*h).fd) };
    }
    unsafe { (*h).is_opened = false };
}
