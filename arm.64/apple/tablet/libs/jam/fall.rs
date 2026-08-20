use core::ffi::c_int;
use core::panic::PanicInfo;

extern "C" {
    fn exit(code: c_int) -> !;
}

#[panic_handler]
fn fall(_info: &PanicInfo) -> ! {
    unsafe { exit(1) }
}
