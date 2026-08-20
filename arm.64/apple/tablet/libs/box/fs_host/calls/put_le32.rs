pub fn put_le32(buf: *mut u8, v: u32) {
    unsafe {
        *buf.add(0) = (v & 0xff) as u8;
        *buf.add(1) = ((v >> 8) & 0xff) as u8;
        *buf.add(2) = ((v >> 16) & 0xff) as u8;
        *buf.add(3) = ((v >> 24) & 0xff) as u8;
    }
}
