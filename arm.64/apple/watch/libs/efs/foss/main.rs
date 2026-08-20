#![no_std]
#![no_main]

extern crate alloc;

mod fall;
mod heap;
mod cstr;

use alloc::string::String;
use alloc::vec::Vec;
use core::ffi::{c_char, c_int, c_void, CStr};

use self::cstr::cstr;

extern "C" {
    fn open(path: *const c_char, flags: c_int) -> c_int;
    fn read(fd: c_int, buf: *mut c_void, count: usize) -> isize;
    fn creat(path: *const c_char, mode: u16) -> c_int;
    fn write(fd: c_int, buf: *const c_void, count: usize) -> isize;
    fn close(fd: c_int) -> c_int;
    fn crc32(crc: u64, buf: *const u8, len: u32) -> u64;
    fn zlibVersion() -> *const c_char;
    fn deflateInit2_(strm: *mut ZStream, level: c_int, method: c_int, window_bits: c_int, mem_level: c_int, strategy: c_int, version: *const c_char, stream_size: c_int) -> c_int;
    fn deflate(strm: *mut ZStream, flush: c_int) -> c_int;
    fn deflateBound(strm: *mut ZStream, source_len: u64) -> u64;
    fn deflateEnd(strm: *mut ZStream) -> c_int;
    fn stat(path: *const c_char, buf: *mut Stat) -> c_int;
}

#[repr(C)]
struct ZStream {
    next_in: *const u8, avail_in: u32, total_in: u64,
    next_out: *mut u8, avail_out: u32, total_out: u64,
    msg: *const u8, state: *mut c_void,
    zalloc: *mut c_void, zfree: *mut c_void, opaque: *mut c_void,
    data_type: c_int, adler: u64, reserved: u64,
}

#[repr(C)]
struct Stat {
    st_dev: i32, st_mode: u16, st_nlink: u16, st_ino: u64,
    st_uid: u32, st_gid: u32, st_rdev: i32,
    st_atime: i64, st_atime_nsec: i64, st_mtime: i64, st_mtime_nsec: i64,
    st_ctime: i64, st_ctime_nsec: i64, st_birthtime: i64, st_birthtime_nsec: i64,
    st_size: i64, st_blocks: i64, st_blksize: i32,
    st_flags: u32, st_gen: u32, st_lspare: i32, st_qspare: [i64; 2],
}

const DOS_TIME: u16 = 0;
const DOS_DATE: u16 = 0x0021;

fn say(msg: &str) {
    unsafe { write(2, msg.as_ptr() as *const c_void, msg.len()) };
}

fn is_file(name: &str) -> bool {
    unsafe {
        let mut st: Stat = core::mem::zeroed();
        if stat(cstr(name).as_ptr() as *const c_char, &mut st) != 0 {
            return false;
        }
        (st.st_mode & 0xf000) == 0x8000
    }
}

fn read_file(name: &str) -> Option<Vec<u8>> {
    let fd = unsafe { open(cstr(name).as_ptr() as *const c_char, 0) };
    if fd < 0 { return None; }
    let mut buf = Vec::new();
    let mut tmp = [0u8; 65536];
    loop {
        let n = unsafe { read(fd, tmp.as_mut_ptr() as *mut c_void, tmp.len()) };
        if n <= 0 { break; }
        buf.extend_from_slice(&tmp[..n as usize]);
    }
    unsafe { close(fd) };
    Some(buf)
}

fn deflate_bytes(input: &[u8], level: c_int) -> Option<Vec<u8>> {
    unsafe {
        let mut s: ZStream = core::mem::zeroed();
        if deflateInit2_(&mut s, level, 8, -15, 8, 0, zlibVersion(), core::mem::size_of::<ZStream>() as c_int) != 0 {
            return None;
        }
        let bound = deflateBound(&mut s, input.len() as u64) as usize;
        let mut out: Vec<u8> = Vec::new();
        out.resize(bound, 0);
        s.next_in = input.as_ptr(); s.avail_in = input.len() as u32;
        s.next_out = out.as_mut_ptr(); s.avail_out = bound as u32;
        let rc = deflate(&mut s, 4);
        let total = s.total_out as usize;
        deflateEnd(&mut s);
        if rc != 1 { return None; }
        out.truncate(total);
        Some(out)
    }
}

fn level_flag(arg: &str) -> Option<c_int> {
    let b = arg.as_bytes();
    if b.len() == 2 && b[0] == b'-' && b[1] >= b'0' && b[1] <= b'9' {
        return Some((b[1] - b'0') as c_int);
    }
    None
}

struct Entry { name: String, crc: u32, comp: u32, orig: u32, off: u32, method: u16 }

fn v16(out: &mut Vec<u8>, v: u16) { out.extend_from_slice(&v.to_le_bytes()); }
fn v32(out: &mut Vec<u8>, v: u32) { out.extend_from_slice(&v.to_le_bytes()); }

#[no_mangle]
extern "C" fn main(argc: c_int, argv: *const *const c_char) -> c_int {
    let mut level: c_int = 6;
    let mut output = "";
    let mut i: isize = 1;
    while i < argc as isize {
        let arg = unsafe { CStr::from_ptr(*argv.offset(i)) }.to_str().unwrap_or("");
        if let Some(l) = level_flag(arg) {
            level = l;
        } else if arg == "-o" && i + 1 < argc as isize {
            i += 1;
            output = unsafe { CStr::from_ptr(*argv.offset(i)) }.to_str().unwrap_or("");
        } else {
            say("foss: unknown arg ");
            say(arg);
            say("\n");
            return 1;
        }
        i += 1;
    }
    if output.is_empty() {
        say("foss: usage: foss [-0..-9] -o output.zip < manifest\n");
        return 1;
    }

    let mut stdin = Vec::new();
    let mut tmp = [0u8; 65536];
    loop {
        let n = unsafe { read(0, tmp.as_mut_ptr() as *mut c_void, tmp.len()) };
        if n <= 0 { break; }
        stdin.extend_from_slice(&tmp[..n as usize]);
    }

    let mut out: Vec<u8> = Vec::new();
    let mut entries: Vec<Entry> = Vec::new();
    for line in stdin.split(|&b| b == b'\n') {
        let mut s = line;
        if s.last() == Some(&b'\r') { s = &s[..s.len() - 1]; }
        if s.is_empty() { continue; }
        let name = match core::str::from_utf8(s) { Ok(n) => n, Err(_) => continue };
        if !is_file(name) { continue; }
        let data = match read_file(name) { Some(d) => d, None => continue };
        let crc = if data.is_empty() {
            0
        } else {
            unsafe { crc32(0, data.as_ptr(), data.len() as u32) as u32 }
        };
        let (method, payload) = if data.is_empty() || level == 0 {
            (0u16, data.clone())
        } else {
            match deflate_bytes(&data, level) {
                Some(z) if z.len() < data.len() => (8u16, z),
                _ => (0u16, data.clone()),
            }
        };
        let off = out.len() as u32;
        v32(&mut out, 0x04034b50);
        v16(&mut out, 20);
        v16(&mut out, 0);
        v16(&mut out, method);
        v16(&mut out, DOS_TIME);
        v16(&mut out, DOS_DATE);
        v32(&mut out, crc);
        v32(&mut out, payload.len() as u32);
        v32(&mut out, data.len() as u32);
        v16(&mut out, name.len() as u16);
        v16(&mut out, 0);
        out.extend_from_slice(name.as_bytes());
        out.extend_from_slice(&payload);
        entries.push(Entry {
            name: String::from(name),
            crc,
            comp: payload.len() as u32,
            orig: data.len() as u32,
            off,
            method,
        });
    }

    let cd_off = out.len() as u32;
    for e in &entries {
        v32(&mut out, 0x02014b50);
        v16(&mut out, 20);
        v16(&mut out, 20);
        v16(&mut out, 0);
        v16(&mut out, e.method);
        v16(&mut out, DOS_TIME);
        v16(&mut out, DOS_DATE);
        v32(&mut out, e.crc);
        v32(&mut out, e.comp);
        v32(&mut out, e.orig);
        v16(&mut out, e.name.len() as u16);
        v16(&mut out, 0);
        v16(&mut out, 0);
        v16(&mut out, 0);
        v16(&mut out, 0);
        v32(&mut out, 0);
        v32(&mut out, e.off);
        out.extend_from_slice(e.name.as_bytes());
    }
    let cd_size = out.len() as u32 - cd_off;
    v32(&mut out, 0x06054b50);
    v16(&mut out, 0);
    v16(&mut out, 0);
    v16(&mut out, entries.len() as u16);
    v16(&mut out, entries.len() as u16);
    v32(&mut out, cd_size);
    v32(&mut out, cd_off);
    v16(&mut out, 0);

    let fd = unsafe { creat(cstr(output).as_ptr() as *const c_char, 0o644) };
    if fd < 0 {
        say("foss: cannot open ");
        say(output);
        say("\n");
        return 1;
    }
    let mut woff = 0usize;
    while woff < out.len() {
        let n = unsafe { write(fd, out[woff..].as_ptr() as *const c_void, out.len() - woff) };
        if n <= 0 { unsafe { close(fd) }; return 1; }
        woff += n as usize;
    }
    unsafe { close(fd) };
    0
}
