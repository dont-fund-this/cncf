use core::ffi::c_int;

use super::type_::Winsize;

const TIOCGWINSZ: u64 = 0x40087468;

extern "C" {
    fn ioctl(fd: c_int, req: u64, ...) -> c_int;
}

pub fn size() -> (usize, usize) {
    let mut ws = Winsize { row: 0, col: 0, x: 0, y: 0 };
    if unsafe { ioctl(1, TIOCGWINSZ, &mut ws as *mut Winsize) } != 0 || ws.col < 10 || ws.row < 7 {
        return (80, 24);
    }
    (ws.col as usize, ws.row as usize)
}
