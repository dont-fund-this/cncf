use super::abi::*;
use super::root_ptr::root_ptr;
use super::super::dev::*;

pub unsafe extern "C" fn boxfs_statfs(fs1: *mut FSDevice, st: *mut FSStatFS) {
    let mut st1: Statfs = unsafe { core::mem::zeroed() };
    let root = root_ptr(fs1);
    if root.is_null() {
        return;
    }
    unsafe { statfs(root, &mut st1) };
    unsafe {
        (*st).f_bsize = st1.f_bsize;
        (*st).f_blocks = st1.f_blocks;
        (*st).f_bfree = st1.f_bfree;
        (*st).f_bavail = st1.f_bavail;
        (*st).f_files = st1.f_files;
        (*st).f_ffree = st1.f_ffree;
    }
}
