use super::super::dev::FSDevice;
use super::super::holder::{slot_of, SLOTS};

pub fn is_ro(fs1: *mut FSDevice) -> bool {
    match slot_of(fs1) {
        Some(i) => unsafe { SLOTS[i].ro },
        None => true,
    }
}
