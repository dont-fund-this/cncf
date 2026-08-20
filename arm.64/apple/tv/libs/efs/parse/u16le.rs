pub fn u16le(b: &[u8], p: usize) -> Option<u16> {
    let s = b.get(p..p + 2)?;
    Some(u16::from_le_bytes([s[0], s[1]]))
}
