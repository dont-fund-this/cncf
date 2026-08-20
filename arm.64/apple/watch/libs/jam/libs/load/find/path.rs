use alloc::string::String;
use core::ffi::c_int;
use super::join::join;
use super::nul::nul;

extern "C" {
    fn _NSGetExecutablePath(buf: *mut u8, size: *mut u32) -> c_int;
}

pub fn path() -> Option<String> {
    let mut buf = [0u8; 1024];
    let mut size = buf.len() as u32;
    if unsafe { _NSGetExecutablePath(buf.as_mut_ptr(), &mut size) } != 0 {
        return None;
    }
    let exe = nul(&buf);
    let dir = &exe[..exe.rfind('/')?];
    Some(join(dir, "Frameworks"))
}
