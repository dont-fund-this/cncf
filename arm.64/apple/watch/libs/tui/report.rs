use crate::type_::Info;

#[no_mangle]
pub unsafe extern "C" fn report() -> Info {
    Info {
        sig: c"tui".as_ptr(),
        tag: c"tui".as_ptr(),
    }
}
