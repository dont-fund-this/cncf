use core::ffi::c_char;
use core::ffi::CStr;

pub type Address = *const c_char;
pub type Payload = *const c_char;
pub type Options = *const c_char;

pub type Sid = *const c_char;
pub type Tag = *const c_char;
pub type Fit = fn(address: Address, payload: Payload, options: Options) -> bool;
pub type Fun = fn(address: Address, payload: Payload, options: Options) -> i32;

#[derive(Copy, Clone)]
#[repr(C)]
pub struct Def {
    pub sid: Sid,
    pub tag: Tag,
    pub fit: Fit,
    pub fun: Fun,
}

unsafe impl Sync for Def {}

pub type Defs = &'static [Def];

#[derive(Copy, Clone)]
#[repr(C)]
pub struct Trip {
    pub address: Address,
    pub payload: Payload,
    pub options: Options,
}

pub const NOT_IMPLEMENTED_JSON: &CStr = c"{\"text\":\"noop\"}";
