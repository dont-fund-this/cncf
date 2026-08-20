use alloc::vec::Vec;
use crate::type_::Def;

static mut DEFS: Vec<Def> = Vec::new();

pub fn defs() -> &'static [Def] {
    unsafe { &*core::ptr::addr_of!(DEFS) }
}

pub(super) fn mutable() -> &'static mut Vec<Def> {
    unsafe { &mut *core::ptr::addr_of_mut!(DEFS) }
}
