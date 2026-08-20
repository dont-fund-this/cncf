#[repr(C)]
pub struct Termios {
    pub c_iflag: u64,
    pub c_oflag: u64,
    pub c_cflag: u64,
    pub c_lflag: u64,
    pub c_cc: [u8; 20],
    pub c_ispeed: u64,
    pub c_ospeed: u64,
}

#[repr(C)]
pub struct Winsize {
    pub row: u16,
    pub col: u16,
    pub x: u16,
    pub y: u16,
}
