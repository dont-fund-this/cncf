use super::hold::VM;

pub fn clear() {
    unsafe {
        *core::ptr::addr_of_mut!(VM) = core::ptr::null_mut();
    }
}
