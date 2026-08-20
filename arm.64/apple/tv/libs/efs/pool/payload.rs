pub fn payload(blob: usize) -> &'static [u8] {
    unsafe {
        let pool = &*core::ptr::addr_of!(super::POOL);
        let b = &pool.blobs[blob];
        core::slice::from_raw_parts(pool.addr.add(b.off), b.stored as usize)
    }
}
