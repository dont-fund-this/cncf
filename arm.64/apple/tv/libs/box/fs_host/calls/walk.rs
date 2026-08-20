use core::ffi::{c_char, c_int};

use super::abi::*;
use super::contained::contained;
use super::root_ptr::root_ptr;
use super::stat_to_qid::stat_to_qid;
use super::super::dev::*;
use super::super::file::*;

pub unsafe extern "C" fn boxfs_walk(
    fs1: *mut FSDevice,
    pf: *mut *mut FSFile,
    qids: *mut FSQID,
    f: *mut FSFile,
    n: c_int,
    names: *mut *mut c_char,
) -> c_int {
    let root = root_ptr(fs1);
    let h = f as *mut Handle;
    let mut path = dup_c(unsafe { (*h).path });
    let mut st: Stat = unsafe { core::mem::zeroed() };
    let mut i = 0;
    while i < n {
        let name = unsafe { *names.add(i as usize) };
        let path1 = compose_path(path, name);
        if unsafe { lstat(path1, &mut st) } != 0 || !contained(path1, root) {
            free_c(path1);
            break;
        }
        free_c(path);
        path = path1;
        stat_to_qid(unsafe { qids.add(i as usize) }, &st);
        i += 1;
    }
    unsafe { *pf = fid_create(path, (*h).uid) as *mut FSFile };
    i
}
