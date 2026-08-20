use alloc::string::String;
use alloc::vec::Vec;

use super::join::join;
use super::path::path;
use super::read::read;

pub fn find() -> Vec<String> {
    let dir = match path() {
        Some(d) => d,
        None => return Vec::new(),
    };
    let mut out = Vec::new();
    for entry in read(&dir) {
        let base = match entry.rsplit('/').next() {
            Some(n) => n,
            None => continue,
        };
        if base.starts_with("lib") && base.ends_with(".framework") {
            let binary = &base[..base.len() - ".framework".len()];
            out.push(join(&entry, binary));
        }
    }
    out
}
