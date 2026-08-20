pub fn u32le(b: &[u8], p: usize) -> Option<u32> {
    let s = b.get(p..p + 4)?;
    Some(u32::from_le_bytes([s[0], s[1], s[2], s[3]]))
}
