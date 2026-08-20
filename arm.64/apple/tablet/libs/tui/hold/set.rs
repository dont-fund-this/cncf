use alloc::string::String;

pub fn set(s: String) {
    unsafe {
        let text = &mut *core::ptr::addr_of_mut!(super::TEXT);
        *text = s;
    }
}
