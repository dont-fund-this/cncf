use crate::type_::InvokeFn;

pub fn keep(cb: Option<InvokeFn>) {
    unsafe {
        super::HOST_INVOKE = cb;
    }
}
