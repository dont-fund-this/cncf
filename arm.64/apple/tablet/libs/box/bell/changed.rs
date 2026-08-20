use super::hold::{PTR, SEEN};

pub fn changed() -> Option<u32> {
    let ptr = unsafe { *core::ptr::addr_of!(PTR) };
    if ptr.is_null() {
        return None;
    }
    let generation = unsafe { core::ptr::read_volatile(ptr as *const u32) };
    let seen = unsafe { *core::ptr::addr_of!(SEEN) };
    if generation == seen {
        return None;
    }
    unsafe { *core::ptr::addr_of_mut!(SEEN) = generation };
    Some(generation)
}
