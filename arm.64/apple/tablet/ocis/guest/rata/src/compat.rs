use core::ffi::c_void;

// libgcc's RISC-V unwinder can use glibc's optional object lookup API. musl
// does not expose it; failure selects libgcc's normal fallback lookup path.
#[no_mangle]
pub extern "C" fn _dl_find_object(_address: *const c_void, _result: *mut c_void) -> i32 {
    -1
}
