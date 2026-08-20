#![no_std]
#![no_main]
#![allow(dead_code, unused_imports, non_upper_case_globals)]

mod defs;
mod halt;
#[path = "impl/mod.rs"]
mod impl_mod;
mod pump;
mod trip;
mod r#type;

use core::ffi::CStr;
use crate::defs::less::less;
use crate::defs::more::more;
use crate::pump::Pump;
use crate::r#type::{Address, Def, Options, Payload};
use crate::trip::trip;

extern "C" {
    fn write(fd: i32, buf: *const u8, count: usize) -> isize;
}

pub fn main() -> i32 {
    let some_def = Def {
        sid: c"some-id".as_ptr(),
        tag: c"thing1".as_ptr(),
        fit: |address: Address, _p: Payload, _o: Options| -> bool {
            if address.is_null() { return false; }
            let addr = unsafe { CStr::from_ptr(address).to_bytes() };
            addr == b"some-id"
        },
        fun: |_a: Address, payload: Payload, _o: Options| -> i32 {
            if !payload.is_null() {
                let bytes = unsafe { CStr::from_ptr(payload).to_bytes() };
                unsafe {
                    write(1, bytes.as_ptr(), bytes.len());
                    write(1, b"\n".as_ptr(), 1);
                }
            }
            0
        },
    };

    let some_other_def = Def {
        sid: c"some-other-id".as_ptr(),
        tag: c"thing2".as_ptr(),
        fit: |address: Address, _p: Payload, _o: Options| -> bool {
            if address.is_null() { return false; }
            let addr = unsafe { CStr::from_ptr(address).to_bytes() };
            addr == b"some-other-id"
        },
        fun: |_a: Address, payload: Payload, _o: Options| -> i32 {
            if !payload.is_null() {
                let bytes = unsafe { CStr::from_ptr(payload).to_bytes() };
                unsafe {
                    write(1, bytes.as_ptr(), bytes.len());
                    write(1, b"\n".as_ptr(), 1);
                }
            }
            0
        },
    };

    more(some_def);
    more(some_other_def);

    for t in trip() {
        Pump(t.address, t.payload, t.options);
    }

    less(some_def);
    less(some_other_def);

    0
}
