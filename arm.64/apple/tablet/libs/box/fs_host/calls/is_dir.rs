use core::ffi::c_char;

use super::abi::*;

pub fn is_dir(root: *const c_char) -> bool {
    if root.is_null() {
        return false;
    }
    let mut st: Stat = unsafe { core::mem::zeroed() };
    unsafe { lstat(root, &mut st) == 0 && (st.st_mode & S_IFMT) == S_IFDIR }
}
