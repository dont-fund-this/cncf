pub fn kind(d: &[u8]) -> &'static str {
    let n = d.len();
    if n == 0 {
        return "empty";
    }
    if n >= 16 && &d[..15] == b"SQLite format 3" {
        return "sqlite";
    }
    if n >= 4 && d[0] == b'P' && d[1] == b'K' && (d[2] == 3 || d[2] == 5) && (d[3] == 4 || d[3] == 6) {
        return "zip";
    }
    if n >= 2 && d[0] == 0x1f && d[1] == 0x8b {
        return "gzip";
    }
    if n >= 5 && &d[..5] == b"%PDF-" {
        return "pdf";
    }
    if n >= 8 && d[0] == 0x89 && &d[1..4] == b"PNG" {
        return "png";
    }
    if n >= 3 && d[0] == 0xff && d[1] == 0xd8 && d[2] == 0xff {
        return "jpeg";
    }
    if n >= 6 && &d[..4] == b"GIF8" {
        return "gif";
    }
    let mut bad = 0usize;
    for &c in d {
        if !(c == 9 || c == 10 || c == 13 || (c >= 32 && c <= 126)) {
            bad += 1;
        }
    }
    if bad * 10 < n * 3 {
        "text"
    } else {
        "bin"
    }
}
