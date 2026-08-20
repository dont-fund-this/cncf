use core::ffi::c_int;
use core::panic::PanicInfo;

extern "C" {
    fn exit(code: c_int) -> !;
}

#[panic_handler]
fn fall(_info: &PanicInfo) -> ! {
    unsafe { exit(1) }
}

#[no_mangle]
pub extern "C" fn rust_eh_personality() {}
