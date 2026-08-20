use core::ffi::CStr;
use crate::type_::Def;

pub fn more(def: Def) {
    let defs = super::defs::mutable();
    let sid = unsafe { CStr::from_ptr(def.sid) }.to_bytes();
    if let Some(index) = defs.iter().position(|item| {
        unsafe { CStr::from_ptr(item.sid) }.to_bytes() == sid
    }) {
        defs[index] = def;
    } else {
        defs.push(def);
    }
}
