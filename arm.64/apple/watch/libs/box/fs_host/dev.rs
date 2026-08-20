use core::ffi::{c_char, c_int, c_void};

pub const P9_QTDIR: u8 = 0x80;
pub const P9_QTSYMLINK: u8 = 0x02;
pub const P9_QTFILE: u8 = 0x00;

pub const P9_O_NOACCESS: c_int = 0x00000003;
pub const P9_O_CREAT: c_int = 0x00000040;
pub const P9_O_EXCL: c_int = 0x00000080;
pub const P9_O_TRUNC: c_int = 0x00000200;
pub const P9_O_APPEND: c_int = 0x00000400;
pub const P9_O_NONBLOCK: c_int = 0x00000800;
pub const P9_O_DSYNC: c_int = 0x00001000;
pub const P9_O_DIRECTORY: c_int = 0x00010000;
pub const P9_O_NOFOLLOW: c_int = 0x00020000;
pub const P9_O_SYNC: c_int = 0x00100000;

pub const P9_SETATTR_MODE: u32 = 0x00000001;
pub const P9_SETATTR_UID: u32 = 0x00000002;
pub const P9_SETATTR_GID: u32 = 0x00000004;
pub const P9_SETATTR_SIZE: u32 = 0x00000008;
pub const P9_SETATTR_ATIME: u32 = 0x00000010;
pub const P9_SETATTR_MTIME: u32 = 0x00000020;
pub const P9_SETATTR_CTIME: u32 = 0x00000040;
pub const P9_SETATTR_ATIME_SET: u32 = 0x00000080;
pub const P9_SETATTR_MTIME_SET: u32 = 0x00000100;

pub const P9_EPERM: c_int = 1;
pub const P9_ENOENT: c_int = 2;
pub const P9_EIO: c_int = 5;
pub const P9_EEXIST: c_int = 17;
pub const P9_EINVAL: c_int = 22;
pub const P9_ENOSPC: c_int = 28;
pub const P9_ENOTEMPTY: c_int = 39;
pub const P9_EPROTO: c_int = 71;
pub const P9_ENOTSUP: c_int = 524;

pub const P9_LOCK_TYPE_RDLCK: u8 = 0;
pub const P9_LOCK_TYPE_WRLCK: u8 = 1;
pub const P9_LOCK_TYPE_UNLCK: u8 = 2;

pub const P9_LOCK_SUCCESS: c_int = 0;
pub const P9_LOCK_BLOCKED: c_int = 1;

#[repr(C)]
pub struct FSStatFS {
    pub f_bsize: u32,
    pub f_blocks: u64,
    pub f_bfree: u64,
    pub f_bavail: u64,
    pub f_files: u64,
    pub f_ffree: u64,
}

#[repr(C)]
pub struct FSQID {
    pub type_: u8,
    pub version: u32,
    pub path: u64,
}

#[repr(C)]
pub struct FSStat {
    pub qid: FSQID,
    pub st_mode: u32,
    pub st_uid: u32,
    pub st_gid: u32,
    pub st_nlink: u64,
    pub st_rdev: u64,
    pub st_size: u64,
    pub st_blksize: u64,
    pub st_blocks: u64,
    pub st_atime_sec: u64,
    pub st_atime_nsec: u32,
    pub st_mtime_sec: u64,
    pub st_mtime_nsec: u32,
    pub st_ctime_sec: u64,
    pub st_ctime_nsec: u32,
}

#[repr(C)]
pub struct FSLock {
    pub type_: u8,
    pub flags: u32,
    pub start: u64,
    pub length: u64,
    pub proc_id: u32,
    pub client_id: *mut c_char,
}

pub type FSOpenCompletionFunc =
    unsafe extern "C" fn(*mut FSDevice, *mut FSQID, c_int, *mut c_void);

#[repr(C)]
pub struct FSDevice {
    pub fs_end: Option<unsafe extern "C" fn(*mut FSDevice)>,
    pub fs_delete: Option<unsafe extern "C" fn(*mut FSDevice, *mut FSFile)>,
    pub fs_statfs: Option<unsafe extern "C" fn(*mut FSDevice, *mut FSStatFS)>,
    pub fs_attach: Option<
        unsafe extern "C" fn(
            *mut FSDevice,
            *mut *mut FSFile,
            *mut FSQID,
            u32,
            *const c_char,
            *const c_char,
        ) -> c_int,
    >,
    pub fs_walk: Option<
        unsafe extern "C" fn(
            *mut FSDevice,
            *mut *mut FSFile,
            *mut FSQID,
            *mut FSFile,
            c_int,
            *mut *mut c_char,
        ) -> c_int,
    >,
    pub fs_mkdir: Option<
        unsafe extern "C" fn(
            *mut FSDevice,
            *mut FSQID,
            *mut FSFile,
            *const c_char,
            u32,
            u32,
        ) -> c_int,
    >,
    pub fs_open: Option<
        unsafe extern "C" fn(
            *mut FSDevice,
            *mut FSQID,
            *mut FSFile,
            u32,
            Option<FSOpenCompletionFunc>,
            *mut c_void,
        ) -> c_int,
    >,
    pub fs_create: Option<
        unsafe extern "C" fn(
            *mut FSDevice,
            *mut FSQID,
            *mut FSFile,
            *const c_char,
            u32,
            u32,
            u32,
        ) -> c_int,
    >,
    pub fs_stat:
        Option<unsafe extern "C" fn(*mut FSDevice, *mut FSFile, *mut FSStat) -> c_int>,
    pub fs_setattr: Option<
        unsafe extern "C" fn(
            *mut FSDevice,
            *mut FSFile,
            u32,
            u32,
            u32,
            u32,
            u64,
            u64,
            u64,
            u64,
            u64,
        ) -> c_int,
    >,
    pub fs_close: Option<unsafe extern "C" fn(*mut FSDevice, *mut FSFile)>,
    pub fs_readdir: Option<
        unsafe extern "C" fn(*mut FSDevice, *mut FSFile, u64, *mut u8, c_int) -> c_int,
    >,
    pub fs_read: Option<
        unsafe extern "C" fn(*mut FSDevice, *mut FSFile, u64, *mut u8, c_int) -> c_int,
    >,
    pub fs_write: Option<
        unsafe extern "C" fn(*mut FSDevice, *mut FSFile, u64, *const u8, c_int) -> c_int,
    >,
    pub fs_link: Option<
        unsafe extern "C" fn(*mut FSDevice, *mut FSFile, *mut FSFile, *const c_char) -> c_int,
    >,
    pub fs_symlink: Option<
        unsafe extern "C" fn(
            *mut FSDevice,
            *mut FSQID,
            *mut FSFile,
            *const c_char,
            *const c_char,
            u32,
        ) -> c_int,
    >,
    pub fs_mknod: Option<
        unsafe extern "C" fn(
            *mut FSDevice,
            *mut FSQID,
            *mut FSFile,
            *const c_char,
            u32,
            u32,
            u32,
            u32,
        ) -> c_int,
    >,
    pub fs_readlink: Option<
        unsafe extern "C" fn(*mut FSDevice, *mut c_char, c_int, *mut FSFile) -> c_int,
    >,
    pub fs_renameat: Option<
        unsafe extern "C" fn(
            *mut FSDevice,
            *mut FSFile,
            *const c_char,
            *mut FSFile,
            *const c_char,
        ) -> c_int,
    >,
    pub fs_unlinkat:
        Option<unsafe extern "C" fn(*mut FSDevice, *mut FSFile, *const c_char) -> c_int>,
    pub fs_lock:
        Option<unsafe extern "C" fn(*mut FSDevice, *mut FSFile, *const FSLock) -> c_int>,
    pub fs_getlock:
        Option<unsafe extern "C" fn(*mut FSDevice, *mut FSFile, *mut FSLock) -> c_int>,
}

pub enum FSFile {}
