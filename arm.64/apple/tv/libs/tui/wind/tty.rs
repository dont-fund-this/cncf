use core::ffi::c_int;

extern "C" {
    fn isatty(fd: c_int) -> c_int;
}

pub fn tty() -> bool {
    unsafe { isatty(0) == 1 && isatty(1) == 1 }
}
