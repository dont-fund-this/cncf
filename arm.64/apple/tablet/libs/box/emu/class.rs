use core::ffi::{c_char, c_int, c_void};

use super::params::VirtMachineParams;

#[repr(C)]
pub struct VirtMachineClass {
    pub machine_names: *const c_char,
    pub virt_machine_set_defaults: Option<unsafe extern "C" fn(*mut VirtMachineParams)>,
    pub virt_machine_init: Option<unsafe extern "C" fn(*const VirtMachineParams) -> *mut VirtMachine>,
    pub virt_machine_end: Option<unsafe extern "C" fn(*mut VirtMachine)>,
    pub virt_machine_get_sleep_duration: Option<unsafe extern "C" fn(*mut VirtMachine, c_int) -> c_int>,
    pub virt_machine_interp: Option<unsafe extern "C" fn(*mut VirtMachine, c_int)>,
    pub vm_mouse_is_absolute: Option<unsafe extern "C" fn(*mut VirtMachine) -> c_int>,
    pub vm_send_mouse_event: Option<unsafe extern "C" fn(*mut VirtMachine, c_int, c_int, c_int, u32)>,
    pub vm_send_key_event: Option<unsafe extern "C" fn(*mut VirtMachine, c_int, u16)>,
}

#[repr(C)]
pub struct VirtMachine {
    pub vmc: *const VirtMachineClass,
    pub net: *mut c_void,
    pub console_dev: *mut c_void,
    pub console: *mut c_void,
    pub fb_dev: *mut c_void,
}
