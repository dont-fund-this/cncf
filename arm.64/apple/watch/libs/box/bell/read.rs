use super::hold::PTR;

pub fn active() -> bool {
    !unsafe { *core::ptr::addr_of!(PTR) }.is_null()
}

pub fn read() -> u32 {
    let ptr = unsafe { *core::ptr::addr_of!(PTR) };
    if ptr.is_null() {
        return 0;
    }
    unsafe { *(ptr as *const u32) }
}
