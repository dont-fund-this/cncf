use core::ffi::{c_int, c_void};

use super::put::put;
use super::size::size;
use super::frame::frame;
use super::type_::Termios;

const ICANON: u64 = 0x100;
const ECHO: u64 = 0x8;
const TCSANOW: c_int = 0;
const VMIN: usize = 16;
const VTIME: usize = 17;

extern "C" {
    fn tcgetattr(fd: c_int, t: *mut Termios) -> c_int;
    fn tcsetattr(fd: c_int, act: c_int, t: *const Termios) -> c_int;
    fn read(fd: c_int, buf: *mut c_void, count: usize) -> isize;
}

pub fn show() {
    let mut old: Termios = unsafe { core::mem::zeroed() };
    let mut raw: Termios = unsafe { core::mem::zeroed() };
    if unsafe { tcgetattr(0, &mut old) } != 0 {
        return;
    }
    if unsafe { tcgetattr(0, &mut raw) } != 0 {
        return;
    }
    raw.c_lflag &= !(ICANON | ECHO);
    raw.c_cc[VMIN] = 1;
    raw.c_cc[VTIME] = 0;
    if unsafe { tcsetattr(0, TCSANOW, &raw) } != 0 {
        return;
    }
    let (cols, rows) = size();
    let held = crate::hold::get();
    let mid = if held.is_empty() {
        alloc::string::String::new()
    } else {
        super::text::text(held.as_ptr() as *const core::ffi::c_char, "tag").unwrap_or_default()
    };
    put("\x1b[?1049h\x1b[?25l\x1b[2J\x1b[H");
    put(&frame(cols, rows, mid.trim_end_matches('\0')));
    loop {
        let mut b: u8 = 0;
        let n = unsafe { read(0, &mut b as *mut u8 as *mut c_void, 1) };
        if n <= 0 || b == b'q' {
            break;
        }
    }
    put("\x1b[?25h\x1b[?1049l");
    unsafe { tcsetattr(0, TCSANOW, &old) };
}
