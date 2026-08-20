use core::ffi::c_char;
use alloc::string::String;

use super::fit::fit;
use super::sid::sid;
use super::render::render;
use crate::path::path;
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
    let dir = match path() {
        Some(d) => d,
        None => return,
    };
    let name = match sid().to_str() {
        Ok(s) => s,
        Err(_) => return,
    };
    let mut dest = String::from(dir);
    dest.push('/');
    dest.push_str(name);
    let body = render();
    save(&dest, body.as_bytes());
}
