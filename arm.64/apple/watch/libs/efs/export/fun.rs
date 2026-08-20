use alloc::string::String;
use core::ffi::c_char;

use super::fit::fit;
use super::text::text;
use super::temp::temp;
use super::dirs::dirs;
use super::err::err;
use super::ack::ack;
use crate::pool::data;
use crate::save::save;

pub unsafe fn fun(
    address: *const c_char,
    payload: *const c_char,
    options: *const c_char,
) {
    let fits = unsafe { fit(address, payload, options) };
    if !fits {
        return;
    }
    let path = match text(payload, "path") {
        Some(p) => p,
        None => {
            err(options, "missing path");
            return;
        }
    };
    let path = path.trim_end_matches('\0');
    if path.is_empty() {
        err(options, "missing path");
        return;
    }
    let bytes = match data(path) {
        Some(b) => b,
        None => {
            let mut msg = String::from("not found: ");
            msg.push_str(path);
            err(options, &msg);
            return;
        }
    };
    let mut dest = temp();
    dest.push('/');
    dest.push_str(path);
    dirs(&dest);
    if !save(&dest, &bytes) {
        err(options, "write failed");
        return;
    }
    ack(options, &dest, bytes.len());
}
