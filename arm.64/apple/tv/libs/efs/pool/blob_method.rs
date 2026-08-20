pub fn blob_method(blob: usize) -> u16 {
    let pool = unsafe { &*core::ptr::addr_of!(super::POOL) };
    pool.blobs[blob].method as u16
}
