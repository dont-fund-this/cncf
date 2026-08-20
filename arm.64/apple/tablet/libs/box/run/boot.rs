use alloc::string::String;
use core::ffi::{c_char, c_int, c_void};

use crate::block::make;
use crate::emu::{
    riscv_machine_class, virt_machine_free_config, virt_machine_init, virt_machine_set_defaults,
    VirtMachineParams, VM_FILE_BIOS, VM_FILE_KERNEL,
};
use crate::fb;
use crate::fs_host;
use crate::log::wire;
use crate::start::Mount;
use crate::vm::set;
use super::dup::dup;
use super::slab::slab;

pub fn boot(
    bios: &String,
    kernel: &String,
    drive: &String,
    cmdline: &String,
    ram: u64,
    mounts: &[Mount],
    fb_w: c_int,
    fb_h: c_int,
) -> bool {
    let kbuf = match slab(kernel.as_ptr() as *const c_char) {
        Some(pair) => pair,
        None => return false,
    };
    let bbuf = match slab(bios.as_ptr() as *const c_char) {
        Some(pair) => pair,
        None => {
            unsafe { freeing(kbuf.0) };
            return false;
        }
    };

    let cd = wire();

    let mut p: VirtMachineParams = unsafe { core::mem::zeroed() };
    unsafe { virt_machine_set_defaults(&mut p) };
    p.vmc = core::ptr::addr_of!(riscv_machine_class) as *const c_void;
    p.machine_name = dup("riscv64");
    let setdef = unsafe { riscv_machine_class.virt_machine_set_defaults };
    if let Some(setdef) = setdef {
        unsafe { setdef(&mut p) };
    }
    p.ram_size = ram << 20;
    p.rtc_real_time = 1;
    p.console = cd as *mut c_void;

    let cmdline_fb = if fb_w > 0 && fb_h > 0 {
        alloc::format!(
            "{} PAT_FB=0x{:x},{}x{} PAT_DOORBELL=0x{:x}\0",
            cmdline.trim_end_matches('\0'),
            crate::fb::ADDR,
            fb_w,
            fb_h,
            crate::bell::ADDR
        )
    } else {
        cmdline.clone()
    };
    unsafe { crate::emu::vm_add_cmdline(&mut p, cmdline_fb.as_ptr() as *const c_char) };

    p.files[VM_FILE_BIOS as usize].buf = bbuf.0;
    p.files[VM_FILE_BIOS as usize].len = bbuf.1;
    p.files[VM_FILE_KERNEL as usize].buf = kbuf.0;
    p.files[VM_FILE_KERNEL as usize].len = kbuf.1;

    let drv = if drive.len() > 1 {
        make(drive.as_ptr() as *const c_char)
    } else {
        core::ptr::null_mut()
    };
    if !drv.is_null() {
        p.tab_drive[0].block_dev = drv as *mut c_void;
        p.drive_count = 1;
    }

    let mut fsn = 0usize;
    for m in mounts.iter().take(fs_host::MAX_FS) {
        let dev = fs_host::make(fsn, m.host.as_ptr() as *const c_char, m.mode_ro);
        if dev.is_null() {
            continue;
        }
        p.tab_fs[fsn].tag = dup(m.tag.trim_end_matches('\0'));
        p.tab_fs[fsn].fs_dev = dev as *mut c_void;
        p.tab_fs[fsn].filename = core::ptr::null_mut();
        p.tab_fs[fsn].device = core::ptr::null_mut();
        fsn += 1;
    }
    p.fs_count = fsn as core::ffi::c_int;

    let vm = unsafe { virt_machine_init(&p) };
    unsafe { virt_machine_free_config(&mut p) };
    if vm.is_null() {
        crate::block::free();
        crate::log::free();
        return false;
    }
    set(vm);
    if fb_w > 0 && fb_h > 0 {
        fb::setup(fb_w, fb_h, vm);
        crate::bell::setup(vm);
    }
    true
}

unsafe fn freeing(p: *mut u8) {
    extern "C" {
        fn free(ptr: *mut c_void);
    }
    unsafe { free(p as *mut c_void) };
}
