use alloc::string::String;
use crate::pool::{entries, data};

pub fn render() -> String {
    let mut out = String::new();
    for e in entries() {
        out.push_str("==> ");
        out.push_str(e.path.as_str());
        out.push_str(" <==\n");
        if let Some(bytes) = data(e.path.as_str()) {
            if let Ok(s) = core::str::from_utf8(&bytes) {
                out.push_str(s);
            }
        }
        out.push('\n');
    }
    out
}
