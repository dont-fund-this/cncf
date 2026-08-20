pub fn ready() -> bool {
    let pool = unsafe { &*core::ptr::addr_of!(super::POOL) };
    pool.ready
}
