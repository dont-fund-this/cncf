use core::ffi::{c_char, c_int, c_void};

extern "C" {
    fn malloc(size: usize) -> *mut c_void;
    fn free(ptr: *mut c_void);
    fn strlen(s: *const c_char) -> usize;
    fn strdup(s: *const c_char) -> *mut c_char;
}

#[repr(C)]
pub struct Handle {
    pub uid: u32,
    pub path: *mut c_char,
    pub is_opened: bool,
    pub is_dir: bool,
    pub fd: c_int,
    pub dirp: *mut c_void,
}

pub fn fid_create(path: *mut c_char, uid: u32) -> *mut Handle {
    let h = unsafe { malloc(core::mem::size_of::<Handle>()) } as *mut Handle;
    if h.is_null() {
        return core::ptr::null_mut();
    }
    unsafe {
        (*h).uid = uid;
        (*h).path = path;
        (*h).is_opened = false;
        (*h).is_dir = false;
        (*h).fd = -1;
        (*h).dirp = core::ptr::null_mut();
    }
    h
}

pub fn dup_c(s: *const c_char) -> *mut c_char {
    unsafe { strdup(s) }
}

pub fn compose_path(path: *const c_char, name: *const c_char) -> *mut c_char {
    let path_len = unsafe { strlen(path) };
    let name_len = unsafe { strlen(name) };
    let nb = name as *const u8;
    let mut has_slash = false;
    let mut k = 0;
    while k < name_len {
        if unsafe { *nb.add(k) } == b'/' {
            has_slash = true;
            break;
        }
        k += 1;
    }
    let is_dot = name_len == 1 && unsafe { *nb.add(0) } == b'.';
    let is_dotdot = name_len == 2 && unsafe { *nb.add(0) } == b'.' && unsafe { *nb.add(1) } == b'.';
    if name_len == 0 || has_slash || is_dot || is_dotdot {
        return core::ptr::null_mut();
    }
    let d = unsafe { malloc(path_len + 1 + name_len + 1) } as *mut c_char;
    if d.is_null() {
        return core::ptr::null_mut();
    }
    unsafe {
        core::ptr::copy_nonoverlapping(path as *const u8, d as *mut u8, path_len);
        *d.add(path_len) = b'/' as c_char;
        core::ptr::copy_nonoverlapping(name as *const u8, d.add(path_len + 1) as *mut u8, name_len + 1);
    }
    d
}

pub fn free_c(p: *mut c_char) {
    unsafe { free(p as *mut c_void) };
}
