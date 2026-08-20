use core::ffi::c_char;

use super::calls::*;
use super::dev::FSDevice;
use super::holder::{MAX_FS, ROOT_MAX, SLOTS};

extern "C" {
    fn strlen(s: *const c_char) -> usize;
}

pub fn make(slot: usize, root: *const c_char, ro: bool) -> *mut FSDevice {
    if slot >= MAX_FS || root.is_null() {
        return core::ptr::null_mut();
    }
    if !is_dir(root) {
        return core::ptr::null_mut();
    }
    let n = unsafe { strlen(root) };
    if n + 1 > ROOT_MAX {
        return core::ptr::null_mut();
    }
    unsafe {
        let s = &mut SLOTS[slot];
        core::ptr::copy_nonoverlapping(root as *const u8, s.root.as_mut_ptr(), n);
        s.root[n] = 0;
        s.ro = ro;
        s.used = true;
        let d = &mut s.dev;
        d.fs_end = Some(boxfs_end);
        d.fs_delete = Some(boxfs_delete);
        d.fs_statfs = Some(boxfs_statfs);
        d.fs_attach = Some(boxfs_attach);
        d.fs_walk = Some(boxfs_walk);
        d.fs_mkdir = Some(boxfs_mkdir);
        d.fs_open = Some(boxfs_open);
        d.fs_create = Some(boxfs_create);
        d.fs_stat = Some(boxfs_stat);
        d.fs_setattr = Some(boxfs_setattr);
        d.fs_close = Some(boxfs_close);
        d.fs_readdir = Some(boxfs_readdir);
        d.fs_read = Some(boxfs_read);
        d.fs_write = Some(boxfs_write);
        d.fs_link = Some(boxfs_link);
        d.fs_symlink = Some(boxfs_symlink);
        d.fs_mknod = Some(boxfs_mknod);
        d.fs_readlink = Some(boxfs_readlink);
        d.fs_renameat = Some(boxfs_renameat);
        d.fs_unlinkat = Some(boxfs_unlinkat);
        d.fs_lock = Some(boxfs_lock);
        d.fs_getlock = Some(boxfs_getlock);
        core::ptr::addr_of_mut!(s.dev)
    }
}

pub fn free_all() {
    let mut i = 0;
    while i < MAX_FS {
        unsafe { SLOTS[i].used = false };
        i += 1;
    }
}
