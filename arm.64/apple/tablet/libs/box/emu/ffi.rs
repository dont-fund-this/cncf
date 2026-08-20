use core::ffi::c_char;

use super::params::VirtMachineParams;
use super::class::{VirtMachine, VirtMachineClass};

extern "C" {
    pub static riscv_machine_class: VirtMachineClass;

    pub fn virt_machine_set_defaults(p: *mut VirtMachineParams);
    pub fn virt_machine_init(p: *const VirtMachineParams) -> *mut VirtMachine;
    pub fn virt_machine_free_config(p: *mut VirtMachineParams);
    pub fn vm_add_cmdline(p: *mut VirtMachineParams, cmdline: *const c_char);
}
