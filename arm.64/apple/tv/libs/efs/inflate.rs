use alloc::vec::Vec;
use core::ffi::{c_char, c_int, c_void};

#[repr(C)]
struct ZStream {
    next_in: *const u8, avail_in: u32, total_in: u64,
    next_out: *mut u8, avail_out: u32, total_out: u64,
    msg: *const u8, state: *mut c_void,
    zalloc: *mut c_void, zfree: *mut c_void, opaque: *mut c_void,
    data_type: c_int, adler: u64, reserved: u64,
}

extern "C" {
    fn zlibVersion() -> *const c_char;
    fn inflateInit2_(strm: *mut ZStream, window_bits: c_int, version: *const c_char, stream_size: c_int) -> c_int;
    fn inflate(strm: *mut ZStream, flush: c_int) -> c_int;
    fn inflateEnd(strm: *mut ZStream) -> c_int;
}

const MAX: usize = 64 * 1024 * 1024;

pub fn inflate_raw(src: &[u8], orig_len: usize) -> Option<Vec<u8>> {
    if orig_len > MAX {
        return None;
    }
    unsafe {
        let mut s: ZStream = core::mem::zeroed();
        if inflateInit2_(&mut s, -15, zlibVersion(), core::mem::size_of::<ZStream>() as c_int) != 0 {
            return None;
        }
        let mut out: Vec<u8> = Vec::new();
        out.resize(orig_len, 0);
        s.next_in = src.as_ptr(); s.avail_in = src.len() as u32;
        s.next_out = out.as_mut_ptr(); s.avail_out = orig_len as u32;
        let rc = inflate(&mut s, 4);
        let got = s.total_out as usize;
        inflateEnd(&mut s);
        if rc != 1 || got != orig_len {
            return None;
        }
        Some(out)
    }
}
