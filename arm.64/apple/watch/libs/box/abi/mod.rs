use crate::type_::InvokeFn;

mod keep;
mod invoke;

pub use keep::keep;
pub use invoke::invoke;

pub(crate) static mut ABI_INVOKE: Option<InvokeFn> = None;
