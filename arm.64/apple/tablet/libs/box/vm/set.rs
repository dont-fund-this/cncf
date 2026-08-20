use crate::emu::VirtMachine;
use super::hold::VM;

pub fn set(vm: *mut VirtMachine) {
    unsafe {
        *core::ptr::addr_of_mut!(VM) = vm;
    }
}
