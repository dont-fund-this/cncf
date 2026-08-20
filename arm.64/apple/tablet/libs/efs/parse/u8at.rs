pub fn u8at(b: &[u8], p: usize) -> Option<u8> {
    b.get(p).copied()
}
