use crate::emu::BlockDevice;

pub unsafe extern "C" fn count(_bs: *mut BlockDevice) -> i64 {
    match unsafe { &*core::ptr::addr_of!(super::hold::BACK) } {
        Some(back) => back.len() as i64 / 512,
        None => 0,
    }
}
