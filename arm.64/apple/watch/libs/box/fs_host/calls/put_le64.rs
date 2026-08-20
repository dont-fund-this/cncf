pub fn put_le64(buf: *mut u8, v: u64) {
    let mut i = 0;
    while i < 8 {
        unsafe { *buf.add(i) = ((v >> (i * 8)) & 0xff) as u8 };
        i += 1;
    }
}
