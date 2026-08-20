use alloc::string::String;
use alloc::vec::Vec;
use core::ffi::{c_char, CStr};

#[derive(Clone, Copy)]
enum Tok<'a> {
    Open,
    Close,
    Colon,
    Comma,
    Str(&'a [u8]),
    True,
    Word,
}

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

fn lex<'a>(src: &'a [u8]) -> Vec<Tok<'a>> {
    let mut toks = Vec::new();
    let mut i = 0;
    while i < src.len() {
        match src[i] {
            b' ' | b'\t' | b'\n' | b'\r' => i += 1,
            b'{' | b'[' => {
                toks.push(Tok::Open);
                i += 1;
            }
            b'}' | b']' => {
                toks.push(Tok::Close);
                i += 1;
            }
            b':' => {
                toks.push(Tok::Colon);
                i += 1;
            }
            b',' => {
                toks.push(Tok::Comma);
                i += 1;
            }
            b'"' => {
                let start = i + 1;
                let mut j = start;
                while j < src.len() && src[j] != b'"' {
                    if src[j] == b'\\' {
                        j += 1;
                    }
                    j += 1;
                }
                toks.push(Tok::Str(&src[start..j.min(src.len())]));
                i = (j + 1).min(src.len());
            }
            _ => {
                let start = i;
                while i < src.len() && !delim(src[i]) {
                    i += 1;
                }
                toks.push(if &src[start..i] == b"true" {
                    Tok::True
                } else {
                    Tok::Word
                });
            }
        }
    }
    toks
}

fn delim(c: u8) -> bool {
    matches!(
        c,
        b' ' | b'\t' | b'\n' | b'\r' | b'{' | b'}' | b'[' | b']' | b':' | b',' | b'"'
    )
}
