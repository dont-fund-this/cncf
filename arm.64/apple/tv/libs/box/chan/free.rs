use super::hold::INPUT;

pub fn free() {
    unsafe {
        *core::ptr::addr_of_mut!(INPUT) = alloc::vec::Vec::new();
    }
}
