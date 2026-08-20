#![allow(non_upper_case_globals)]

use core::ffi::CStr;
use crate::pump::into::{get_into, has_verb};
use crate::pump::Pump;
use crate::r#type::{Address, Def, Options, Payload};

pub static VersionGet: Def = Def {
    sid: c"version".as_ptr(),
    tag: c"tag,any".as_ptr(),
    fit: |address: Address, _payload: Payload, options: Options| -> bool {
        if address.is_null() { return false; }
        let addr = unsafe { CStr::from_ptr(address).to_bytes() };
        (addr == b"/version" || addr == b"version") && has_verb(options, b"GET")
    },
    fun: |address: Address, payload: Payload, options: Options| -> i32 {
        if !(VersionGet.fit)(address, payload, options) { return -1; }
        let json_payload = concat!(include_str!("get.json"), "\0").as_ptr() as Payload;
        if let Some(into_target) = get_into(options) {
            return Pump(into_target, json_payload, c"once".as_ptr());
        }
        1
    },
};
