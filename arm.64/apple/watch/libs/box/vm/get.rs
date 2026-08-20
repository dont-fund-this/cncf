use crate::emu::VirtMachine;
use super::hold::VM;

pub fn get() -> *mut VirtMachine {
    unsafe { *core::ptr::addr_of!(VM) }
}
