use alloc::string::String;
use alloc::vec::Vec;
use core::ffi::{c_char, CStr};

use super::lex::{lex, Tok};

pub struct Mount {
    pub tag: String,
    pub host: String,
    pub mode_ro: bool,
}

pub fn mounts(payload: *const c_char) -> Vec<Mount> {
    let json = unsafe { CStr::from_ptr(payload) };
    let toks = lex(json.to_bytes());
    let mut out: Vec<Mount> = Vec::new();

    let mut i = 0;
    let mut depth: i32 = 0;
    let mut start = usize::MAX;
    while i < toks.len() {
        match toks[i] {
            Tok::Open => depth += 1,
            Tok::Close => depth -= 1,
            Tok::Str(k) if depth == 1 && k == b"mounts" => {
                if matches!(toks.get(i + 1), Some(Tok::Colon))
                    && matches!(toks.get(i + 2), Some(Tok::Open))
                {
                    start = i + 2;
                    break;
                }
            }
            _ => {}
        }
        i += 1;
    }
    if start == usize::MAX {
        return out;
    }

    let mut j = start;
    let mut d: i32 = 0;
    let mut tag: Option<String> = None;
    let mut host: Option<String> = None;
    let mut mode_ro = true;
    while j < toks.len() {
        match toks[j] {
            Tok::Open => {
                d += 1;
                if d == 2 {
                    tag = None;
                    host = None;
                    mode_ro = true;
                }
            }
            Tok::Close => {
                d -= 1;
                if d == 1 {
                    if let (Some(t), Some(h)) = (tag.take(), host.take()) {
                        if out.len() < crate::fs_host::MAX_FS {
                            out.push(Mount {
                                tag: t,
                                host: h,
                                mode_ro,
                            });
                        }
                    }
                }
                if d == 0 {
                    break;
                }
            }
            Tok::Str(k) if d == 2 => {
                if matches!(toks.get(j + 1), Some(Tok::Colon)) {
                    if let Some(Tok::Str(v)) = toks.get(j + 2).copied() {
                        if k == b"tag" {
                            tag = Some(nul(v));
                        } else if k == b"host" {
                            host = Some(nul(v));
                        } else if k == b"mode" {
                            mode_ro = v == b"ro";
                        }
                    }
                }
            }
            _ => {}
        }
        j += 1;
    }
    out
}

fn nul(v: &[u8]) -> String {
    let mut s = String::from(core::str::from_utf8(v).unwrap_or(""));
    s.push('\0');
    s
}
