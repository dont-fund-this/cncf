use std::ffi::CStr;

use crate::abi::Def;

static mut DEFS: Vec<Def> = Vec::new();

pub fn defs() -> &'static [Def] {
    unsafe { &*std::ptr::addr_of!(DEFS) }
}

pub fn more(def: Def) {
    let defs = unsafe { &mut *std::ptr::addr_of_mut!(DEFS) };
    if let Some(index) = defs.iter().position(|item| item.sid == def.sid) {
        defs[index] = def;
    } else {
        defs.push(def);
    }
}

pub fn less(sid: &CStr) {
    unsafe { &mut *std::ptr::addr_of_mut!(DEFS) }
        .retain(|item| item.sid.as_c_str() != sid);
}
