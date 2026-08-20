use core::ffi::{c_char, c_int, c_uint, CStr};

use crate::type_::Def;

pub fn term() -> Def {
    Def {
        sid: c"tui.term".as_ptr(),
        tag: c"term".as_ptr(),
        fun,
        fit,
    }
}

unsafe fn fit(address: *const c_char, _payload: *const c_char, _options: *const c_char) -> bool {
    let a = unsafe { CStr::from_ptr(address) };
    a == c"tui.term" || a == c"term"
}

unsafe fn fun(address: *const c_char, payload: *const c_char, options: *const c_char) {
    if !unsafe { fit(address, payload, options) } {
        return;
    }
    extern "C" {
        fn isatty(fd: c_int) -> c_int;
        fn usleep(us: c_uint) -> c_int;
    }
    if unsafe { isatty(1) } != 1 {
        return;
    }
    let empty = c"{}";
    let opt = c"{\"strict\":\"once\",\"into\":\"tui.out\"}";
    let mut i = 0;
    while i < 240 {
        crate::abi::invoke(c"box.poll".as_ptr(), empty.as_ptr(), opt.as_ptr());
        unsafe { usleep(15000) };
        i += 1;
    }
}
