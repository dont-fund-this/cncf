use crate::type_::Info;

#[no_mangle]
pub unsafe extern "C" fn report() -> Info {
    Info {
        sig: c"efs".as_ptr(),
        tag: c"efs".as_ptr(),
    }
}
