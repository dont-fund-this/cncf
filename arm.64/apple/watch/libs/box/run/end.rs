use crate::emu::VirtMachine;
use crate::vm::{clear, get};

pub fn end() {
    let vm: *mut VirtMachine = get();
    let mut vm_exited = false;
    if !vm.is_null() {
        let class = unsafe { (*vm).vmc };
        if !class.is_null() {
            if let Some(finish) = unsafe { (*class).virt_machine_end } {
                unsafe { finish(vm) };
                vm_exited = true;
            }
        }
    }
    clear();
    crate::fb::free(vm_exited);
    crate::bell::free(vm_exited);
    crate::chan::free();
    crate::block::free();
    crate::fs_host::free_all();
    crate::log::free();
}
