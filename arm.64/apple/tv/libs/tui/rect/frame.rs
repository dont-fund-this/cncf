use alloc::string::String;

pub fn frame(cols: usize, rows: usize, mid: &str) -> String {
    let mut out = String::new();
    let center = rows / 2;
    let quit = rows / 2 + 2;
    for r in 1..=rows {
        if r == 1 || r == rows {
            out.push('+');
            for _ in 0..cols - 2 {
                out.push('-');
            }
            out.push('+');
        } else {
            let inner = cols - 2;
            let text = if r == center {
                if mid.len() >= inner {
                    &mid[..inner]
                } else {
                    mid
                }
            } else if r == quit {
                "q quit"
            } else {
                ""
            };
            let left = (inner - text.len()) / 2;
            out.push('|');
            for _ in 0..left {
                out.push(' ');
            }
            out.push_str(text);
            for _ in 0..inner - left - text.len() {
                out.push(' ');
            }
            out.push('|');
        }
        if r < rows {
            out.push_str("\r\n");
        }
    }
    out
}
