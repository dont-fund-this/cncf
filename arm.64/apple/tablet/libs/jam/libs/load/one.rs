use core::ffi::CStr;
use crate::type_::InvokeFn;

pub enum Outcome {
    Ignored,
    Attached,
    Rejected,
}

pub fn one(path: &str, want: &str) -> Outcome {
    let abi = match super::bind::bind(path) {
        Some(abi) => abi,
        None => return Outcome::Ignored,
    };
    unsafe {
        let info = (abi.report)();
        let sig = CStr::from_ptr(info.sig).to_bytes();
        let tag = CStr::from_ptr(info.tag).to_bytes();
        if sig != want.as_bytes() || tag != want.as_bytes() {
            super::bind::close(abi);
            return Outcome::Ignored;
        }
        let jam_invoke: Option<InvokeFn> = Some(crate::invoke::invoke);
        if !(abi.attach)(jam_invoke) {
            (abi.detach)();
            super::bind::close(abi);
            return Outcome::Rejected;
        }
        let libs = &mut *core::ptr::addr_of_mut!(super::LIBS);
        libs.push(abi);
    }
    Outcome::Attached
}
