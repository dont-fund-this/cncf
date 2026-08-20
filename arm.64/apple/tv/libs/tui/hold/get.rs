use alloc::string::String;

pub fn get() -> String {
    unsafe {
        let text = &*core::ptr::addr_of!(super::TEXT);
        text.clone()
    }
}
