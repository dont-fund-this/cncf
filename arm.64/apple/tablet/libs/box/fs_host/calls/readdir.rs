use core::ffi::{c_char, c_int};

use super::abi::*;
use super::put_le16::put_le16;
use super::put_le32::put_le32;
use super::put_le64::put_le64;
use super::super::dev::*;
use super::super::file::*;

pub unsafe extern "C" fn boxfs_readdir(
    fs1: *mut FSDevice,
    f: *mut FSFile,
    offset: u64,
    buf: *mut u8,
    count: c_int,
) -> c_int {
    let _ = fs1;
    let h = f as *mut Handle;
    if unsafe { !(*h).is_opened || !(*h).is_dir } {
        return -P9_EPROTO;
    }
    let dirp = unsafe { (*h).dirp };
    if offset == 0 {
        unsafe { rewinddir(dirp) };
    } else {
        unsafe { seekdir(dirp, offset as i64) };
    }
    let mut pos: c_int = 0;
    loop {
        let de = unsafe { readdir(dirp) };
        if de.is_null() {
            break;
        }
        let name_ptr = unsafe { core::ptr::addr_of!((*de).d_name) as *const c_char };
        let name_len = unsafe { strlen(name_ptr) } as c_int;
        let len = 13 + 8 + 1 + 2 + name_len;
        if pos + len > count {
            break;
        }
        let off = unsafe { telldir(dirp) };
        let mut d_type = unsafe { (*de).d_type };
        if d_type == DT_UNKNOWN {
            let path = compose_path(unsafe { (*h).path }, name_ptr);
            let mut st: Stat = unsafe { core::mem::zeroed() };
            d_type = if unsafe { lstat(path, &mut st) } == 0 {
                (st.st_mode >> 12) as u8
            } else {
                8
            };
            free_c(path);
        }
        let ty = if d_type == DT_DIR {
            P9_QTDIR
        } else if d_type == DT_LNK {
            P9_QTSYMLINK
        } else {
            P9_QTFILE
        };
        unsafe {
            *buf.add(pos as usize) = ty;
            pos += 1;
            put_le32(buf.add(pos as usize), 0);
            pos += 4;
            put_le64(buf.add(pos as usize), (*de).d_ino);
            pos += 8;
            put_le64(buf.add(pos as usize), off as u64);
            pos += 8;
            *buf.add(pos as usize) = d_type;
            pos += 1;
            put_le16(buf.add(pos as usize), name_len as u16);
            pos += 2;
            core::ptr::copy_nonoverlapping(name_ptr as *const u8, buf.add(pos as usize), name_len as usize);
            pos += name_len;
        }
    }
    pos
}
