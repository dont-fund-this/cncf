use libloading::Library;
use std::os::raw::c_char;

pub type Address = *const c_char;
pub type Payload = *const c_char;
pub type Options = *const c_char;
pub type Sid = *const c_char;
pub type Tag = *const c_char;

pub type Fit = unsafe extern "C" fn(Address, Payload, Options) -> bool;
pub type Fun = unsafe extern "C" fn(Address, Payload, Options) -> i32;

#[repr(C)]
pub struct Def {
    pub sid: Sid,
    pub tag: Tag,
    pub fit: Fit,
    pub fun: Fun,
}

pub type MoreFn = unsafe extern "C" fn(Def) -> i32;
pub type PumpFn = unsafe extern "C" fn(Address, Payload, Options) -> i32;
pub type LessFn = unsafe extern "C" fn(Def) -> i32;

pub struct Cabi {
    pub name: String,
    pub path: String,
    pub lib:  Library,
    pub more: Option<MoreFn>,
    pub pump: PumpFn,
    pub less: Option<LessFn>,
}

pub struct Triplet {
    pub address: &'static str,
    pub payload: &'static str,
    pub options: &'static str,
}
