use alloc::vec::Vec;
use super::type_::{Blob, Entry};

pub fn hold(addr: *const u8, blobs: Vec<Blob>, entries: Vec<Entry>) {
    unsafe {
        let pool = &mut *core::ptr::addr_of_mut!(super::POOL);
        pool.addr = addr;
        pool.blobs = blobs;
        pool.entries = entries;
        pool.ready = true;
    }
}
