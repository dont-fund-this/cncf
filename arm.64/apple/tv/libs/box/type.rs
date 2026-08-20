use core::ffi::c_char;
use core::ffi::c_int;
use core::ffi::c_void;

#[repr(C)]
pub struct Def {
    pub sid: *const c_char,
    pub tag: *const c_char,
    pub fit: FitFn,
    pub fun: FunFn,
}

pub type FunFn = unsafe fn(
    address: *const c_char,
    payload: *const c_char,
    options: *const c_char,
);

pub type FitFn = unsafe fn(
    address: *const c_char,
    payload: *const c_char,
    options: *const c_char,
) -> bool;

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
    pub report: ReportFn,
}
