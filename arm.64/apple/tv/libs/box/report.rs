use crate::type_::Info;

#[no_mangle]
pub unsafe extern "C" fn report() -> Info {
    Info {
        sig: c"box".as_ptr(),
        tag: c"box".as_ptr(),
    }
}
