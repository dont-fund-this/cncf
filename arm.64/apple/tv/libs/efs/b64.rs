use alloc::string::String;

const A: &[u8; 64] = b"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

pub fn b64(data: &[u8]) -> String {
    let mut out = String::new();
    let mut i = 0;
    while i + 3 <= data.len() {
        let n = (data[i] as u32) << 16 | (data[i + 1] as u32) << 8 | data[i + 2] as u32;
        out.push(A[(n >> 18 & 63) as usize] as char);
        out.push(A[(n >> 12 & 63) as usize] as char);
        out.push(A[(n >> 6 & 63) as usize] as char);
        out.push(A[(n & 63) as usize] as char);
        i += 3;
    }
    match data.len() - i {
        1 => {
            let n = (data[i] as u32) << 16;
            out.push(A[(n >> 18 & 63) as usize] as char);
            out.push(A[(n >> 12 & 63) as usize] as char);
            out.push('=');
            out.push('=');
        }
        2 => {
            let n = (data[i] as u32) << 16 | (data[i + 1] as u32) << 8;
            out.push(A[(n >> 18 & 63) as usize] as char);
            out.push(A[(n >> 12 & 63) as usize] as char);
            out.push(A[(n >> 6 & 63) as usize] as char);
            out.push('=');
        }
        _ => {}
    }
    out
}
