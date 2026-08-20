pub fn blob_stored(blob: usize) -> u32 {
    let pool = unsafe { &*core::ptr::addr_of!(super::POOL) };
    pool.blobs[blob].stored
}
