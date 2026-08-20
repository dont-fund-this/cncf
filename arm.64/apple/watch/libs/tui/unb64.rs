use alloc::vec::Vec;

pub fn unb64(s: &str) -> Vec<u8> {
    let mut out = Vec::new();
    let mut acc: u32 = 0;
    let mut have: u32 = 0;
    for b in s.bytes() {
        if b == b'=' {
            break;
        }
        let v = match b {
            b'A'..=b'Z' => b - b'A',
            b'a'..=b'z' => b - b'a' + 26,
            b'0'..=b'9' => b - b'0' + 52,
            b'+' => 62,
            b'/' => 63,
            _ => continue,
        };
        acc = (acc << 6) | v as u32;
        have += 1;
        if have == 4 {
            out.push((acc >> 16) as u8);
            out.push((acc >> 8) as u8);
            out.push(acc as u8);
            acc = 0;
            have = 0;
        }
    }
    if have == 3 {
        out.push((acc >> 10) as u8);
        out.push((acc >> 2) as u8);
    }
    if have == 2 {
        out.push((acc >> 4) as u8);
    }
    out
}
