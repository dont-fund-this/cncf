use super::dev::FSDevice;

pub const MAX_FS: usize = 4;
pub const ROOT_MAX: usize = 1024;

pub struct Slot {
    pub used: bool,
    pub ro: bool,
    pub root: [u8; ROOT_MAX],
    pub dev: FSDevice,
}

const DEV0: FSDevice = FSDevice {
    fs_end: None,
    fs_delete: None,
    fs_statfs: None,
    fs_attach: None,
    fs_walk: None,
    fs_mkdir: None,
    fs_open: None,
    fs_create: None,
    fs_stat: None,
    fs_setattr: None,
    fs_close: None,
    fs_readdir: None,
    fs_read: None,
    fs_write: None,
    fs_link: None,
    fs_symlink: None,
    fs_mknod: None,
    fs_readlink: None,
    fs_renameat: None,
    fs_unlinkat: None,
    fs_lock: None,
    fs_getlock: None,
};

const SLOT0: Slot = Slot {
    used: false,
    ro: false,
    root: [0u8; ROOT_MAX],
    dev: DEV0,
};

pub static mut SLOTS: [Slot; MAX_FS] = [SLOT0, SLOT0, SLOT0, SLOT0];

pub fn slot_of(fs1: *mut FSDevice) -> Option<usize> {
    let mut i = 0;
    while i < MAX_FS {
        let d = unsafe { core::ptr::addr_of_mut!(SLOTS[i].dev) };
        if d == fs1 {
            return Some(i);
        }
        i += 1;
    }
    None
}
