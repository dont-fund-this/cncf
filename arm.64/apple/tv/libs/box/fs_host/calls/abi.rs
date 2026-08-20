use core::ffi::{c_char, c_int, c_void};

use crate::emu::Dirent;

pub const O_CREAT: c_int = 512;
pub const O_EXCL: c_int = 2048;
pub const O_TRUNC: c_int = 1024;
pub const O_APPEND: c_int = 8;
pub const O_NONBLOCK: c_int = 4;
pub const O_DSYNC: c_int = 4194304;
pub const O_NOFOLLOW: c_int = 256;
pub const O_SYNC: c_int = 128;

pub const F_SETLK: c_int = 8;
pub const F_GETLK: c_int = 7;
pub const F_RDLCK: i16 = 1;
pub const F_UNLCK: i16 = 2;
pub const F_WRLCK: i16 = 3;
pub const SEEK_SET: c_int = 0;
pub const AT_FDCWD: c_int = -2;
pub const AT_SYMLINK_NOFOLLOW: c_int = 32;
pub const UTIME_NOW: i64 = -1;
pub const UTIME_OMIT: i64 = -2;

pub const E_PERM: c_int = 1;
pub const E_NOENT: c_int = 2;
pub const E_IO: c_int = 5;
pub const E_EXIST: c_int = 17;
pub const E_INVAL: c_int = 22;
pub const E_NOSPC: c_int = 28;
pub const E_NOTEMPTY: c_int = 66;
pub const E_AGAIN: c_int = 35;
pub const E_ACCES: c_int = 13;
pub const E_PROTO: c_int = 100;
pub const E_NOTSUP: c_int = 45;

pub const S_IFMT: u16 = 61440;
pub const S_IFDIR: u16 = 16384;
pub const S_IFLNK: u16 = 40960;

pub const DT_UNKNOWN: u8 = 0;
pub const DT_DIR: u8 = 4;
pub const DT_LNK: u8 = 10;

#[repr(C)]
pub struct Timespec {
    pub tv_sec: i64,
    pub tv_nsec: i64,
}

#[repr(C)]
pub struct Stat {
    pub st_dev: i32,
    pub st_mode: u16,
    pub st_nlink: u16,
    pub st_ino: u64,
    pub st_uid: u32,
    pub st_gid: u32,
    pub st_rdev: i32,
    pub st_atimespec: Timespec,
    pub st_mtimespec: Timespec,
    pub st_ctimespec: Timespec,
    pub st_birthtimespec: Timespec,
    pub st_size: i64,
    pub st_blocks: i64,
    pub st_blksize: i32,
    pub st_flags: u32,
    pub st_gen: u32,
    pub st_lspare: i32,
    pub st_qspare: [i64; 2],
}

#[repr(C)]
pub struct Statfs {
    pub f_bsize: u32,
    pub f_iosize: i32,
    pub f_blocks: u64,
    pub f_bfree: u64,
    pub f_bavail: u64,
    pub f_files: u64,
    pub f_ffree: u64,
    pub _rest: [u8; 2168 - 48],
}

#[repr(C)]
pub struct Flock {
    pub l_start: i64,
    pub l_len: i64,
    pub l_pid: i32,
    pub l_type: i16,
    pub l_whence: i16,
}

extern "C" {
    pub fn __error() -> *mut c_int;
    pub fn lstat(path: *const c_char, st: *mut Stat) -> c_int;
    pub fn statfs(path: *const c_char, st: *mut Statfs) -> c_int;
    pub fn open(path: *const c_char, flags: c_int, ...) -> c_int;
    pub fn close(fd: c_int) -> c_int;
    pub fn opendir(path: *const c_char) -> *mut c_void;
    pub fn closedir(dirp: *mut c_void) -> c_int;
    pub fn readdir(dir: *mut c_void) -> *const Dirent;
    pub fn rewinddir(dirp: *mut c_void);
    pub fn seekdir(dirp: *mut c_void, loc: i64);
    pub fn telldir(dirp: *mut c_void) -> i64;
    pub fn pread(fd: c_int, buf: *mut c_void, count: usize, off: i64) -> isize;
    pub fn pwrite(fd: c_int, buf: *const c_void, count: usize, off: i64) -> isize;
    pub fn mkdir(path: *const c_char, mode: u16) -> c_int;
    pub fn mknod(path: *const c_char, mode: u16, dev: i32) -> c_int;
    pub fn symlink(target: *const c_char, path: *const c_char) -> c_int;
    pub fn readlink(path: *const c_char, buf: *mut c_char, size: usize) -> isize;
    pub fn link(a: *const c_char, b: *const c_char) -> c_int;
    pub fn rename(a: *const c_char, b: *const c_char) -> c_int;
    pub fn remove(path: *const c_char) -> c_int;
    pub fn truncate(path: *const c_char, len: i64) -> c_int;
    pub fn chmod(path: *const c_char, mode: u16) -> c_int;
    pub fn lchown(path: *const c_char, uid: u32, gid: u32) -> c_int;
    pub fn utimensat(fd: c_int, path: *const c_char, times: *const Timespec, flag: c_int) -> c_int;
    pub fn fcntl(fd: c_int, cmd: c_int, ...) -> c_int;
    pub fn strlen(s: *const c_char) -> usize;
    pub fn free(ptr: *mut c_void);
    pub fn realpath(path: *const c_char, resolved: *mut c_char) -> *mut c_char;
}
