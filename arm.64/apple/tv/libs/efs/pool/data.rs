use alloc::vec::Vec;
use super::payload::payload;
use crate::inflate::inflate_raw;

pub fn data(path: &str) -> Option<Vec<u8>> {
    if !crate::load::map() {
        return None;
    }
    let pool = unsafe { &*core::ptr::addr_of!(super::POOL) };
    for e in pool.entries.iter() {
        let e_path = e.path.as_str();
        if e_path == path || (e_path.starts_with("refs/") && &e_path[5..] == path) {
            let b = &pool.blobs[e.blob];
            let raw = payload(e.blob);
            return if b.method == 8 {
                inflate_raw(raw, b.orig as usize)
            } else {
                let mut v = Vec::new();
                v.extend_from_slice(raw);
                Some(v)
            };
        }
    }
    None
}
