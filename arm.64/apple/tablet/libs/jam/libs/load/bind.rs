use core::ffi::{c_char, c_int, c_void};
use core::mem::transmute;
use crate::type_::Abi;
use super::cstr::cstr;

const RTLD_NOW: c_int = 0x2;
const RTLD_LOCAL: c_int = 0x4;

extern "C" {
    fn dlopen(path: *const c_char, mode: c_int) -> *mut c_void;
    fn dlsym(handle: *mut c_void, symbol: *const c_char) -> *mut c_void;
    fn dlclose(handle: *mut c_void) -> c_int;
}

pub fn bind(path: &str) -> Option<Abi> {
    let lib = unsafe { dlopen(cstr(path).as_ptr() as *const c_char, RTLD_NOW | RTLD_LOCAL) };
    if lib.is_null() {
        return None;
    }
    let attach = unsafe { dlsym(lib, c"attach".as_ptr()) };
    let detach = unsafe { dlsym(lib, c"detach".as_ptr()) };
    let invoke = unsafe { dlsym(lib, c"invoke".as_ptr()) };
    let report = unsafe { dlsym(lib, c"report".as_ptr()) };
    if attach.is_null() || detach.is_null() || invoke.is_null() || report.is_null() {
        unsafe { dlclose(lib) };
        return None;
    }
    let abi = unsafe {
        Abi {
            lib,
            attach: transmute(attach),
            detach: transmute(detach),
            invoke: transmute(invoke),
            report: transmute(report),
        }
    };
    let info = unsafe { (abi.report)() };
    if !info.sig.is_null() && !info.tag.is_null() {
        Some(abi)
    } else {
        unsafe { dlclose(lib) };
        None
    }
}

pub fn close(abi: Abi) {
    unsafe { dlclose(abi.lib) };
}
