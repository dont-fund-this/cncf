use crate::emu::VirtMachine;
use crate::vm::get;

pub fn tick() {
    let vm: *mut VirtMachine = get();
    if vm.is_null() {
        return;
    }
    let class = unsafe { (*vm).vmc };
    if class.is_null() {
        return;
    }
    crate::chan::pump();
    if let Some(interp) = unsafe { (*class).virt_machine_interp } {
        unsafe { interp(vm, 100000) };
    }
}
