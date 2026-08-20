use crate::type_::Abi;

pub fn libs() -> &'static [Abi] {
    unsafe { &*core::ptr::addr_of!(super::LIBS) }
}
