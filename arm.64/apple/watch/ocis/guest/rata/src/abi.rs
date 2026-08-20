use std::ffi::{c_char, c_int, c_void, CString};

pub struct Def {
    pub sid: CString,
    pub tag: CString,
    pub fit: FitFn,
    pub fun: FunFn,
}

pub type FitFn = Box<dyn Fn(*const c_char, *const c_char, *const c_char) -> bool>;
pub type FunFn = Box<dyn Fn(*const c_char, *const c_char, *const c_char)>;

#[repr(C)]
pub struct Info {
    pub sig: *const c_char,
    pub tag: *const c_char,
}

pub type InvokeFn = unsafe extern "C" fn(*const c_char, *const c_char, *const c_char) -> c_int;
pub type AttachFn = unsafe extern "C" fn(Option<InvokeFn>) -> bool;
pub type DetachFn = unsafe extern "C" fn() -> bool;
pub type ReportFn = unsafe extern "C" fn() -> Info;

pub struct Abi {
    pub lib: *mut c_void,
    pub attach: AttachFn,
    pub detach: DetachFn,
    pub invoke: InvokeFn,
}
