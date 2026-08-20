use std::cell::RefCell;
use std::ffi::{c_char, CStr, CString};
use std::rc::Rc;

use crate::abi::Def;

pub fn post(address: &str, payload: &str, strict: &str) -> Vec<String> {
    if !matches!(strict, "none" | "once" | "many") {
        return Vec::new();
    }
    let Some(sid) = uuid() else { return Vec::new() };
    let (Ok(address), Ok(payload), Ok(options)) = (
        CString::new(address),
        CString::new(payload),
        CString::new(format!(
        "{{\"strict\":\"{}\",\"into\":\"{}\"}}",
        strict,
        sid.to_string_lossy(),
        )),
    ) else { return Vec::new() };
    let replies = Rc::new(RefCell::new(Vec::new()));
    let fit_sid = sid.clone();
    let receive = Rc::clone(&replies);
    crate::defs::more(Def {
        sid: sid.clone(),
        tag: sid.clone(),
        fit: Box::new(move |address, _, _| {
            !address.is_null() && unsafe { CStr::from_ptr(address) == fit_sid.as_c_str() }
        }),
        fun: Box::new(move |_, payload, _| {
            if !payload.is_null() {
                receive.borrow_mut().push(
                    unsafe { CStr::from_ptr(payload) }.to_string_lossy().into_owned(),
                );
            }
        }),
    });
    crate::jam::invoke(&address, &payload, &options);
    crate::defs::less(sid.as_c_str());
    let result = replies.borrow().clone();
    result
}

fn uuid() -> Option<CString> {
    let source = std::fs::read_to_string("/proc/sys/kernel/random/uuid").ok()?;
    let text = source.trim();
    if text.len() != 36 {
        return None;
    }
    for (index, byte) in text.bytes().enumerate() {
        if matches!(index, 8 | 13 | 18 | 23) {
            if byte != b'-' {
                return None;
            }
        } else if !byte.is_ascii_hexdigit() {
            return None;
        }
    }
    CString::new(text).ok()
}

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
    if options.is_null() {
        return None;
    }
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
                        return Some(String::from(std::str::from_utf8(v).unwrap_or("")));
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
