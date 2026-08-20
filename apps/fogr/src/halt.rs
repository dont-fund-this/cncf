use core::panic::PanicInfo;

extern "C" {
    fn exit(code: i32) -> !;
}

#[panic_handler]
fn halt(_info: &PanicInfo) -> ! {
    unsafe {
        exit(1);
    }
}

#[no_mangle]
pub extern "C" fn main() -> i32 {
    crate::main()
}
