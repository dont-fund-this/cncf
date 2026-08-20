use alloc::vec::Vec;
use crate::pool::{entries, payload, blob_method, blob_crc, blob_stored, blob_orig};
use super::w16::w16;
use super::w32::w32;

pub fn frame() -> Vec<u8> {
    let mut out: Vec<u8> = Vec::new();
    let ents = entries();
    let mut offs: Vec<u32> = Vec::with_capacity(ents.len());

    for e in ents {
        offs.push(out.len() as u32);
        w32(&mut out, 0x04034b50);
        w16(&mut out, 20);
        w16(&mut out, 0);
        w16(&mut out, blob_method(e.blob));
        w16(&mut out, e.time);
        w16(&mut out, e.date);
        w32(&mut out, blob_crc(e.blob));
        w32(&mut out, blob_stored(e.blob));
        w32(&mut out, blob_orig(e.blob));
        w16(&mut out, e.path.len() as u16);
        w16(&mut out, 0);
        out.extend_from_slice(e.path.as_bytes());
        out.extend_from_slice(payload(e.blob));
    }

    let cd_off = out.len() as u32;
    for (i, e) in ents.iter().enumerate() {
        w32(&mut out, 0x02014b50);
        w16(&mut out, 20);
        w16(&mut out, 20);
        w16(&mut out, 0);
        w16(&mut out, blob_method(e.blob));
        w16(&mut out, e.time);
        w16(&mut out, e.date);
        w32(&mut out, blob_crc(e.blob));
        w32(&mut out, blob_stored(e.blob));
        w32(&mut out, blob_orig(e.blob));
        w16(&mut out, e.path.len() as u16);
        w16(&mut out, 0);
        w16(&mut out, 0);
        w16(&mut out, 0);
        w16(&mut out, 0);
        w32(&mut out, 0);
        w32(&mut out, offs[i]);
        out.extend_from_slice(e.path.as_bytes());
    }
    let cd_size = out.len() as u32 - cd_off;

    w32(&mut out, 0x06054b50);
    w16(&mut out, 0);
    w16(&mut out, 0);
    w16(&mut out, ents.len() as u16);
    w16(&mut out, ents.len() as u16);
    w32(&mut out, cd_size);
    w32(&mut out, cd_off);
    w16(&mut out, 0);
    out
}
