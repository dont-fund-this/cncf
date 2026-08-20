use crate::emu::CharacterDevice;
use super::hold::CHAR;
use super::sink::sink;
use super::void::void;

pub fn wire() -> *mut CharacterDevice {
    unsafe {
        let cd = &mut *core::ptr::addr_of_mut!(CHAR);
        cd.opaque = core::ptr::null_mut();
        cd.write_data = Some(sink);
        cd.read_data = Some(void);
        core::ptr::addr_of_mut!(CHAR)
    }
}
