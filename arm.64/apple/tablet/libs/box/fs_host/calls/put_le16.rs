pub fn put_le16(buf: *mut u8, v: u16) {
    unsafe {
        *buf.add(0) = (v & 0xff) as u8;
        *buf.add(1) = ((v >> 8) & 0xff) as u8;
    }
}
