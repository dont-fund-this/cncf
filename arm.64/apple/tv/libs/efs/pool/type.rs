use alloc::string::String;
use alloc::vec::Vec;

pub struct Blob {
    pub orig: u32,
    pub stored: u32,
    pub crc: u32,
    pub method: u8,
    pub off: usize,
}

pub struct Entry {
    pub path: String,
    pub blob: usize,
    pub time: u16,
    pub date: u16,
}

pub struct Pool {
    pub addr: *const u8,
    pub blobs: Vec<Blob>,
    pub entries: Vec<Entry>,
    pub ready: bool,
}
