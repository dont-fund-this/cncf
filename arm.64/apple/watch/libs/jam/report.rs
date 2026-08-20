use crate::type_::Info;

#[no_mangle]
pub unsafe extern "C" fn report() -> Info {
    Info {
        sig: c"jam".as_ptr(),
        tag: c"jam".as_ptr(),
    }
}
