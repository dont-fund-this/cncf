use alloc::string::String;
use alloc::vec::Vec;
use crate::inflate::inflate_raw;
use crate::pool::{Blob, Entry};
use super::u8at::u8at;
use super::u16le::u16le;
use super::u32le::u32le;

pub fn parse(bytes: &[u8]) -> Option<(Vec<Blob>, Vec<Entry>)> {
    if bytes.len() < 12 || &bytes[0..4] != b"PATZ" {
        return None;
    }
    let cat_z = u32le(bytes, 4)? as usize;
    let cat_o = u32le(bytes, 8)? as usize;
    let cat = inflate_raw(bytes.get(12..12 + cat_z)?, cat_o)?;
    let pond = 12 + cat_z;

    let mut p = 0usize;
    let blob_count = u32le(&cat, p)? as usize;
    p += 4;
    let mut blobs: Vec<Blob> = Vec::new();
    let mut off = pond;
    for _ in 0..blob_count {
        let orig = u32le(&cat, p)?;
        let stored = u32le(&cat, p + 4)?;
        let crc = u32le(&cat, p + 8)?;
        let method = u8at(&cat, p + 12)?;
        p += 13;
        blobs.push(Blob { orig, stored, crc, method, off });
        off += stored as usize;
    }

    let entry_count = u32le(&cat, p)? as usize;
    p += 4;
    let mut entries: Vec<Entry> = Vec::new();
    let mut prev = String::new();
    for _ in 0..entry_count {
        let k = u8at(&cat, p)? as usize;
        let suf = u16le(&cat, p + 1)? as usize;
        p += 3;
        let head = prev.get(..k)?;
        let mut path = String::from(head);
        let sufbytes = cat.get(p..p + suf)?;
        if let Ok(s) = core::str::from_utf8(sufbytes) {
            path.push_str(s);
        }
        p += suf;
        let blob = u32le(&cat, p)? as usize;
        if blob >= blobs.len() {
            return None;
        }
        let time = u16le(&cat, p + 4)?;
        let date = u16le(&cat, p + 6)?;
        p += 8;
        prev = path.clone();
        entries.push(Entry { path, blob, time, date });
    }

    Some((blobs, entries))
}
