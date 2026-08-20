use super::super::dev::*;
use super::super::holder::{slot_of, SLOTS};

pub unsafe extern "C" fn boxfs_end(fs1: *mut FSDevice) {
    if let Some(i) = slot_of(fs1) {
        unsafe { SLOTS[i].used = false };
    }
}
