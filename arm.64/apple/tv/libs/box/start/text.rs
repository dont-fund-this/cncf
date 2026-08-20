use alloc::string::String;
use core::ffi::{c_char, CStr};

use super::lex::{lex, Tok};

pub fn text(options: *const c_char, key: &str) -> Option<String> {
    let json = unsafe { CStr::from_ptr(options) };
    let toks = lex(json.to_bytes());
    let key = key.as_bytes();

    let mut depth: i32 = 0;
    for i in 0..toks.len() {
        match toks[i] {
            Tok::Open => depth += 1,
            Tok::Close => depth -= 1,
            Tok::Str(k) if depth == 1 && k == key => {
                let member = matches!(toks.get(i + 1), Some(Tok::Colon));
                if member {
                    if let Some(Tok::Str(v)) = toks.get(i + 2).copied() {
                        let mut s = String::from(core::str::from_utf8(v).unwrap_or(""));
                        s.push('\0');
                        return Some(s);
                    }
                }
            }
            _ => {}
        }
    }
    None
}
