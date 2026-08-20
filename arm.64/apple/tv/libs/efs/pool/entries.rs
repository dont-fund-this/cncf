use super::type_::Entry;

pub fn entries() -> &'static [Entry] {
    unsafe { &(*core::ptr::addr_of!(super::POOL)).entries }
}
