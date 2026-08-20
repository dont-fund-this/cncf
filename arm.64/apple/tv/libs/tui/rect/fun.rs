use core::ffi::c_char;

use super::fit::fit;
use super::tty::tty;
use super::show::show;

pub unsafe fn fun(
    address: *const c_char,
    payload: *const c_char,
    options: *const c_char,
) {
    let fits = unsafe { fit(address, payload, options) };
    if !fits {
        return;
    }
    crate::abi::invoke(
        c"efs.read".as_ptr(),
        c"{\"path\": \"refs/rect/rect.json\"}".as_ptr(),
        c"{\"strict\":\"once\",\"into\":\"tui.rect.take\"}".as_ptr(),
    );
    if !tty() {
        return;
    }
    show();
}
