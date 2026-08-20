use core::ffi::c_int;

use super::abi::__error;

pub fn errno() -> c_int {
    unsafe { *__error() }
}
