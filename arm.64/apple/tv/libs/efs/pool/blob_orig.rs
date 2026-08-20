pub fn blob_orig(blob: usize) -> u32 {
    let pool = unsafe { &*core::ptr::addr_of!(super::POOL) };
    pool.blobs[blob].orig
}
