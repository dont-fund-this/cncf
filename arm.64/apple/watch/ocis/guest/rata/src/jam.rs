use std::ffi::{c_void, CStr, CString};
use std::mem::transmute;

use crate::abi::{Abi, AttachFn, DetachFn, InvokeFn, ReportFn};

static mut JAM: Option<Abi> = None;

pub fn prep() -> bool {
    if unsafe { (&*std::ptr::addr_of!(JAM)).is_some() } {
        return true;
    }
    let path = match std::fs::read_link("/proc/self/exe") {
        Ok(path) => path.with_file_name("libjam.so"),
        Err(_) => return false,
    };
    let path = match CString::new(path.to_string_lossy().as_bytes()) {
        Ok(path) => path,
        Err(_) => return false,
    };
    let lib = unsafe { libc::dlopen(path.as_ptr(), libc::RTLD_NOW | libc::RTLD_LOCAL) };
    if lib.is_null() {
        let error = unsafe { libc::dlerror() };
        if !error.is_null() {
            eprintln!("JAM-DLOPEN {}", unsafe { CStr::from_ptr(error) }.to_string_lossy());
        }
        return false;
    }
    let attach = unsafe { libc::dlsym(lib, c"attach".as_ptr()) };
    let detach = unsafe { libc::dlsym(lib, c"detach".as_ptr()) };
    let invoke = unsafe { libc::dlsym(lib, c"invoke".as_ptr()) };
    let report = unsafe { libc::dlsym(lib, c"report".as_ptr()) };
    if attach.is_null() || detach.is_null() || invoke.is_null() || report.is_null() {
        unsafe { libc::dlclose(lib) };
        return false;
    }
    let report: ReportFn = unsafe { transmute(report) };
    let info = unsafe { report() };
    if info.sig.is_null() || info.tag.is_null()
        || unsafe { CStr::from_ptr(info.sig) } != c"jam"
        || unsafe { CStr::from_ptr(info.tag) } != c"jam"
    {
        unsafe { libc::dlclose(lib) };
        return false;
    }
    let abi = Abi {
        lib: lib as *mut c_void,
        attach: unsafe { transmute::<*mut c_void, AttachFn>(attach) },
        detach: unsafe { transmute::<*mut c_void, DetachFn>(detach) },
        invoke: unsafe { transmute::<*mut c_void, InvokeFn>(invoke) },
    };
    if !unsafe { (abi.attach)(Some(crate::invoke::invoke)) } {
        unsafe { libc::dlclose(lib) };
        return false;
    }
    unsafe { *std::ptr::addr_of_mut!(JAM) = Some(abi) };
    true
}

pub fn invoke(address: &CStr, payload: &CStr, options: &CStr) -> i32 {
    match unsafe { &*std::ptr::addr_of!(JAM) } {
        Some(jam) => unsafe { (jam.invoke)(address.as_ptr(), payload.as_ptr(), options.as_ptr()) },
        None => 0,
    }
}

pub fn detach() {
    if let Some(jam) = unsafe { (&mut *std::ptr::addr_of_mut!(JAM)).take() } {
        unsafe {
            (jam.detach)();
            libc::dlclose(jam.lib);
        }
    }
}
