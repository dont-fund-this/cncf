use super::hold::INPUT;

pub fn push(bytes: &[u8]) -> usize {
    if bytes.is_empty() {
        return 0;
    }
    let input = unsafe { &mut *core::ptr::addr_of_mut!(INPUT) };
    input.extend_from_slice(bytes);
    bytes.len()
}
