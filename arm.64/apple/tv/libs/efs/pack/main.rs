#![no_std]
#![no_main]

extern crate alloc;

mod fall;
mod heap;
mod cstr;

use alloc::string::String;
use alloc::vec::Vec;
use alloc::collections::BTreeMap;
use core::ffi::{c_char, c_int, c_void, CStr};

use self::cstr::cstr;

extern "C" {
    fn open(path: *const c_char, flags: c_int) -> c_int;
    fn read(fd: c_int, buf: *mut c_void, count: usize) -> isize;
    fn creat(path: *const c_char, mode: u16) -> c_int;
    fn write(fd: c_int, buf: *const c_void, count: usize) -> isize;
    fn close(fd: c_int) -> c_int;
    fn crc32(crc: u64, buf: *const u8, len: u32) -> u64;
    fn zlibVersion() -> *const c_char;
    fn deflateInit2_(strm: *mut ZStream, level: c_int, method: c_int, window_bits: c_int, mem_level: c_int, strategy: c_int, version: *const c_char, stream_size: c_int) -> c_int;
    fn deflate(strm: *mut ZStream, flush: c_int) -> c_int;
    fn deflateBound(strm: *mut ZStream, source_len: u64) -> u64;
    fn deflateEnd(strm: *mut ZStream) -> c_int;
    fn stat(path: *const c_char, buf: *mut Stat) -> c_int;
    fn getenv(name: *const c_char) -> *mut c_char;
}

#[repr(C)]
struct ZStream {
    next_in: *const u8, avail_in: u32, total_in: u64,
    next_out: *mut u8, avail_out: u32, total_out: u64,
    msg: *const u8, state: *mut c_void,
    zalloc: *mut c_void, zfree: *mut c_void, opaque: *mut c_void,
    data_type: c_int, adler: u64, reserved: u64,
}

#[repr(C)]
struct Stat {
    st_dev: i32, st_mode: u16, st_nlink: u16, st_ino: u64,
    st_uid: u32, st_gid: u32, st_rdev: i32,
    st_atime: i64, st_atime_nsec: i64, st_mtime: i64, st_mtime_nsec: i64,
    st_ctime: i64, st_ctime_nsec: i64, st_birthtime: i64, st_birthtime_nsec: i64,
    st_size: i64, st_blocks: i64, st_blksize: i32,
    st_flags: u32, st_gen: u32, st_lspare: i32, st_qspare: [i64; 2],
}

fn resolve_path(name: &str) -> String {
    unsafe {
        let ptr = getenv(cstr("ROOT").as_ptr() as *const c_char);
        if ptr.is_null() {
            String::from(name)
        } else {
            let root_str = CStr::from_ptr(ptr).to_str().unwrap_or("");
            if root_str.is_empty() {
                String::from(name)
            } else {
                let mut path = String::from(root_str);
                if !path.ends_with('/') && !name.starts_with('/') {
                    path.push('/');
                }
                path.push_str(name);
                path
            }
        }
    }
}

fn read_file(name: &str) -> Option<Vec<u8>> {
    let resolved = resolve_path(name);
    let fd = unsafe { open(cstr(&resolved).as_ptr() as *const c_char, 0) };
    if fd < 0 { return None; }
    let mut buf = Vec::new();
    let mut tmp = [0u8; 65536];
    loop {
        let n = unsafe { read(fd, tmp.as_mut_ptr() as *mut c_void, tmp.len()) };
        if n <= 0 { break; }
        buf.extend_from_slice(&tmp[..n as usize]);
    }
    unsafe { close(fd) };
    Some(buf)
}

fn deflate_bytes(input: &[u8]) -> Option<Vec<u8>> {
    unsafe {
        let mut s: ZStream = core::mem::zeroed();
        if deflateInit2_(&mut s, 6, 8, -15, 8, 0, zlibVersion(), core::mem::size_of::<ZStream>() as c_int) != 0 {
            return None;
        }
        let bound = deflateBound(&mut s, input.len() as u64) as usize;
        let mut out: Vec<u8> = Vec::new();
        out.resize(bound, 0);
        s.next_in = input.as_ptr(); s.avail_in = input.len() as u32;
        s.next_out = out.as_mut_ptr(); s.avail_out = bound as u32;
        let rc = deflate(&mut s, 4);
        let total = s.total_out as usize;
        deflateEnd(&mut s);
        if rc != 1 { return None; }
        out.truncate(total);
        Some(out)
    }
}

fn mtime(name: &str) -> i64 {
    unsafe {
        let resolved = resolve_path(name);
        let mut st: Stat = core::mem::zeroed();
        if stat(cstr(&resolved).as_ptr() as *const c_char, &mut st) != 0 { return 0; }
        st.st_mtime
    }
}

fn dos(epoch: i64) -> (u16, u16) {
    let days = epoch.div_euclid(86400);
    let secs = epoch.rem_euclid(86400);
    let (y, m, d) = civil(days);
    let time = ((secs / 3600) as u16) << 11 | (((secs % 3600) / 60) as u16) << 5 | ((secs % 60) as u16 / 2);
    let year = if y < 1980 { 0u16 } else { (y - 1980) as u16 };
    let date = (year << 9) | ((m as u16) << 5) | (d as u16);
    (time, date)
}

fn civil(z: i64) -> (i64, u32, u32) {
    let z = z + 719468;
    let era = (if z >= 0 { z } else { z - 146096 }) / 146097;
    let doe = (z - era * 146097) as u64;
    let yoe = (doe - doe / 1460 + doe / 36524 - doe / 146096) / 365;
    let y = yoe as i64 + era * 400;
    let doy = doe - (365 * yoe + yoe / 4 - yoe / 100);
    let mp = (5 * doy + 2) / 153;
    let d = (doy - (153 * mp + 2) / 5 + 1) as u32;
    let m = (if mp < 10 { mp + 3 } else { mp - 9 }) as u32;
    (if m <= 2 { y + 1 } else { y }, m, d)
}

struct Blob { orig: Vec<u8>, crc: u32 }
struct Entry { name: String, blob: u32, time: u16, date: u16 }

fn v16(out: &mut Vec<u8>, v: u16) { out.extend_from_slice(&v.to_le_bytes()); }
fn v32(out: &mut Vec<u8>, v: u32) { out.extend_from_slice(&v.to_le_bytes()); }

fn print_stdout(msg: &str) {
    unsafe { write(1, msg.as_ptr() as *const c_void, msg.len()) };
}

fn write_all(fd: c_int, data: &[u8]) -> bool {
    let mut off = 0usize;
    while off < data.len() {
        let n = unsafe { write(fd, data[off..].as_ptr() as *const c_void, data.len() - off) };
        if n <= 0 {
            return false;
        }
        off += n as usize;
    }
    true
}

fn contains(data: &[u8], needle: &[u8]) -> bool {
    if needle.is_empty() || data.len() < needle.len() {
        return false;
    }
    data.windows(needle.len()).any(|part| part == needle)
}

fn text(
    out_path: &str,
    entries: &[Entry],
    blobs: &[Blob],
) -> bool {
    const SEP: &[u8] = b"\xe0\xae\xb4\xe0\xae\xb4\xe0\xae\xb4";
    let logical = entries
        .iter()
        .map(|entry| blobs[entry.blob as usize].orig.len())
        .sum::<usize>();
    let unique = blobs.iter().map(|blob| blob.orig.len()).sum::<usize>();
    let fd = unsafe { creat(cstr(out_path).as_ptr() as *const c_char, 0o644) };
    if fd < 0 {
        return false;
    }
    let header = alloc::format!(
        "PAT-TXT-1\nfiles\t{}\nblobs\t{}\nlogical_bytes\t{}\nunique_bytes\t{}\n",
        entries.len(), blobs.len(), logical, unique,
    );
    let mut ok = write_all(fd, header.as_bytes());
    for entry in entries {
        let line = alloc::format!("{}\t{}\n", entry.name, entry.blob);
        ok = ok && write_all(fd, line.as_bytes());
    }
    ok = ok && write_all(fd, SEP) && write_all(fd, b"\n");
    for (id, blob) in blobs.iter().enumerate() {
        let raw = core::str::from_utf8(&blob.orig).is_ok() && !contains(&blob.orig, SEP);
        let mode = if raw { "raw" } else { "hex" };
        let line = alloc::format!("blob\t{}\t{}\t{}\n", id, blob.orig.len(), mode);
        ok = ok && write_all(fd, line.as_bytes());
        if raw {
            ok = ok && write_all(fd, &blob.orig);
        } else {
            let mut encoded = Vec::with_capacity(blob.orig.len() * 2);
            const HEX: &[u8; 16] = b"0123456789abcdef";
            for &byte in &blob.orig {
                encoded.push(HEX[(byte >> 4) as usize]);
                encoded.push(HEX[(byte & 15) as usize]);
            }
            ok = ok && write_all(fd, &encoded);
        }
        if blob.orig.last() != Some(&b'\n') || !raw {
            ok = ok && write_all(fd, b"\n");
        }
        ok = ok && write_all(fd, SEP) && write_all(fd, b"\n");
    }
    unsafe { close(fd) };
    if ok {
        print_stdout(&alloc::format!(
            "pack: files {} blobs {} logical {} unique {}\n",
            entries.len(), blobs.len(), logical, unique,
        ));
    }
    ok
}

fn count_loc(data: &[u8]) -> usize {
    let s = match core::str::from_utf8(data) {
        Ok(v) => v,
        Err(_) => return 0,
    };
    let mut count = 0;
    for line in s.lines() {
        if !line.trim().is_empty() {
            count += 1;
        }
    }
    count
}

fn safe(name: &str) -> bool {
    if name.is_empty() || name.starts_with('/') || name.contains('\t') || name.contains('\r') {
        return false;
    }
    for part in name.split('/') {
        if part.is_empty() || part == "." || part == ".." || part == ".git" || part == ".DS_Store" || part == "build" || part == "dist" {
            return false;
        }
    }
    true
}

#[no_mangle]
extern "C" fn main(argc: c_int, argv: *const *const c_char) -> c_int {
    let out_path = unsafe { CStr::from_ptr(*argv.offset(1)) }.to_str().unwrap_or("");

    let mut stdin = Vec::new();
    let mut tmp = [0u8; 65536];
    loop {
        let n = unsafe { read(0, tmp.as_mut_ptr() as *mut c_void, tmp.len()) };
        if n <= 0 { break; }
        stdin.extend_from_slice(&tmp[..n as usize]);
    }

    let mut names: Vec<&str> = Vec::new();
    for line in stdin.split(|&b| b == b'\n') {
        let mut s = line;
        if s.last() == Some(&b'\r') { s = &s[..s.len() - 1]; }
        if s.is_empty() { continue; }
        let name = match core::str::from_utf8(s) { Ok(n) => n, Err(_) => continue };
        names.push(name);
    }
    names.sort();
    names.dedup();
    for name in &names {
        if !safe(name) {
            print_stdout(&alloc::format!("pack: unsafe or generated path: {}\n", name));
            return 1;
        }
    }

    let mut blobs: Vec<Blob> = Vec::new();
    let mut entries: Vec<Entry> = Vec::new();
    for name in names {
        let data = match read_file(name) { Some(d) => d, None => continue };
        let (time, date) = dos(mtime(name));
        let crc = unsafe { crc32(0, data.as_ptr(), data.len() as u32) as u32 };
        let mut blob = blobs.len();
        for i in 0..blobs.len() {
            if blobs[i].crc == crc && blobs[i].orig == data { blob = i; break; }
        }
        if blob == blobs.len() { blobs.push(Blob { orig: data, crc }); }
        entries.push(Entry { name: String::from(name), blob: blob as u32, time, date });
    }

    if out_path.ends_with(".txt") {
        return if text(out_path, &entries, &blobs) { 0 } else { 1 };
    }

    let mut pond: Vec<u8> = Vec::new();
    let mut meta: Vec<(u32, u32, u32, u8)> = Vec::new();
    for b in &blobs {
        let (method, payload) = match deflate_bytes(&b.orig) {
            Some(d) if d.len() < b.orig.len() => (8u8, d),
            _ => (0u8, b.orig.clone()),
        };
        meta.push((b.orig.len() as u32, payload.len() as u32, b.crc, method));
        pond.extend_from_slice(&payload);
    }

    let mut cat: Vec<u8> = Vec::new();
    v32(&mut cat, blobs.len() as u32);
    for &(orig, stored, crc, method) in &meta {
        v32(&mut cat, orig); v32(&mut cat, stored); v32(&mut cat, crc); cat.push(method);
    }
    v32(&mut cat, entries.len() as u32);
    let mut prev = "";
    for e in &entries {
        let name = e.name.as_str();
        let mut k = 0usize;
        let (nb, pb) = (name.as_bytes(), prev.as_bytes());
        while k < nb.len() && k < pb.len() && k < 255 && nb[k] == pb[k] { k += 1; }
        cat.push(k as u8);
        v16(&mut cat, (nb.len() - k) as u16);
        cat.extend_from_slice(&nb[k..]);
        v32(&mut cat, e.blob);
        v16(&mut cat, e.time);
        v16(&mut cat, e.date);
        prev = name;
    }
    let mut out: Vec<u8> = Vec::new();
    if out_path.ends_with("/code.zip") {
        struct ZipEntry { name: String, crc: u32, comp: u32, orig: u32, off: u32, method: u16, time: u16, date: u16 }
        let mut zip_entries = Vec::<ZipEntry>::new();
        for e in &entries {
            let name = e.name.as_str();
            let data = &blobs[e.blob as usize].orig;
            let crc = blobs[e.blob as usize].crc;
            let (method, payload) = match deflate_bytes(data) {
                Some(z) if z.len() < data.len() => (8u16, z),
                _ => (0u16, data.clone()),
            };
            let off = out.len() as u32;
            v32(&mut out, 0x04034b50);
            v16(&mut out, 20); v16(&mut out, 0); v16(&mut out, method);
            v16(&mut out, e.time); v16(&mut out, e.date);
            v32(&mut out, crc); v32(&mut out, payload.len() as u32); v32(&mut out, data.len() as u32);
            v16(&mut out, name.len() as u16); v16(&mut out, 0);
            out.extend_from_slice(name.as_bytes());
            out.extend_from_slice(&payload);
            zip_entries.push(ZipEntry {
                name: String::from(name),
                crc,
                comp: payload.len() as u32,
                orig: data.len() as u32,
                off,
                method,
                time: e.time,
                date: e.date,
            });
        }
        let cd_off = out.len() as u32;
        for e in &zip_entries {
            v32(&mut out, 0x02014b50);
            v16(&mut out, 20); v16(&mut out, 20); v16(&mut out, 0);
            v16(&mut out, e.method); v16(&mut out, e.time); v16(&mut out, e.date);
            v32(&mut out, e.crc); v32(&mut out, e.comp); v32(&mut out, e.orig);
            v16(&mut out, e.name.len() as u16); v16(&mut out, 0); v16(&mut out, 0); v16(&mut out, 0); v16(&mut out, 0);
            v32(&mut out, 0); v32(&mut out, e.off);
            out.extend_from_slice(e.name.as_bytes());
        }
        let cd_size = out.len() as u32 - cd_off;
        v32(&mut out, 0x06054b50);
        v16(&mut out, 0); v16(&mut out, 0);
        v16(&mut out, zip_entries.len() as u16); v16(&mut out, zip_entries.len() as u16);
        v32(&mut out, cd_size); v32(&mut out, cd_off);
        v16(&mut out, 0);
    } else {
        let cat_z = match deflate_bytes(&cat) { Some(z) => z, None => return 1 };
        out.extend_from_slice(b"PATZ");
        v32(&mut out, cat_z.len() as u32);
        v32(&mut out, cat.len() as u32);
        out.extend_from_slice(&cat_z);
        out.extend_from_slice(&pond);
    }

    let fd = unsafe { creat(cstr(out_path).as_ptr() as *const c_char, 0o644) };
    if fd < 0 { return 1; }
    let mut off = 0usize;
    while off < out.len() {
        let n = unsafe { write(fd, out[off..].as_ptr() as *const c_void, out.len() - off) };
        if n <= 0 { unsafe { close(fd) }; return 1; }
        off += n as usize;
    }
    unsafe { close(fd) };

    if out_path.ends_with("/code.zip") {
        generate_stats(argc, argv, out_path, &entries, &blobs, &out);
    }
    0
}

fn get_targets() -> Vec<String> {
    let mut targets = Vec::new();
    let content = match read_file("env.mk") {
        Some(c) => c,
        None => match read_file("env.full.mk") {
            Some(c) => c,
            None => return targets,
        }
    };
    let content_str = match core::str::from_utf8(&content) {
        Ok(s) => s,
        Err(_) => return targets,
    };
    for word in content_str.split_whitespace() {
        if word.contains('.') && (
            word.ends_with("/desk") || 
            word.ends_with("/phone") || 
            word.ends_with("/tablet") || 
            word.ends_with("/tv") || 
            word.ends_with("/watch") ||
            word.ends_with("/alpi") ||
            word.ends_with("/busi") ||
            word.ends_with("/pid1")
        ) {
            targets.push(String::from(word));
        }
    }
    targets
}

fn generate_stats(
    _argc: c_int,
    _argv: *const *const c_char,
    _out_path: &str,
    entries: &[Entry],
    blobs: &[Blob],
    out: &[u8],
) {
    let targets = get_targets();
    if !targets.is_empty() {
        let mut desk_target = String::new();
        for t in &targets {
            if t.ends_with("/desk") || t.ends_with("/swift/alpi") {
                desk_target = t.clone();
                break;
            }
        }
        
        if !desk_target.is_empty() {
            let mut cell_files = BTreeMap::<String, Vec<(String, u32)>>::new();
            for e in entries {
                for t in &targets {
                    if e.name.starts_with(t) {
                        let parts: Vec<&str> = t.split('/').collect();
                        if parts.len() >= 3 {
                            let cell_name = if parts[2] == "desk" {
                                String::from(parts[1]) + "/desk"
                            } else if parts[2] == "metal" && parts.len() >= 4 && parts[3] == "desk" {
                                String::from(parts[1]) + "/metal/desk"
                            } else if parts.len() >= 4 {
                                String::from(parts[2]) + "/" + parts[3]
                            } else {
                                continue;
                            };
                            cell_files.entry(cell_name).or_insert_with(Vec::new).push((e.name.clone(), e.blob));
                            break;
                        }
                    }
                }
            }
            
            let mut cells_out = Vec::<String>::new();
            let mut cell_records = Vec::<(String, usize, usize)>::new();
            for (cell_name, files) in &cell_files {
                let mut unique_blobs = 0;
                let mut cell_blobs = Vec::<u32>::new();
                for &(_, b) in files {
                    if !cell_blobs.contains(&b) {
                        cell_blobs.push(b);
                    }
                }
                for &b in &cell_blobs {
                    let mut other = false;
                    for (c2, fs2) in &cell_files {
                        if c2 != cell_name {
                            if fs2.iter().any(|&(_, b2)| b2 == b) {
                                other = true;
                                break;
                            }
                        }
                    }
                    if !other {
                        unique_blobs += 1;
                    }
                }
                cell_records.push((cell_name.clone(), files.len(), unique_blobs));
                cells_out.push(alloc::format!("  - name: {}\n    files: {}\n    unique_blobs: {}", cell_name, files.len(), unique_blobs));
            }
            
            let mut fuzz_list = Vec::<String>::new();
            let mut hs = BTreeMap::<String, Vec<u32>>::new();
            for e in entries {
                for t in &targets {
                    if e.name.starts_with(t) {
                        let parts: Vec<&str> = t.split('/').collect();
                        if parts.len() >= 3 {
                            let plat = parts[2];
                            if plat == "apple" || plat == "droid" {
                                let rel = &e.name[t.len() + 1..];
                                if rel == "env.mk" || rel == "src/input.swift" || rel == "src/input.rs" || rel.ends_with("/env.mk") {
                                    continue;
                                }
                                let key = String::from(plat) + "/" + rel;
                                hs.entry(key).or_insert_with(Vec::new).push(e.blob);
                            }
                            break;
                        }
                    }
                }
            }
            
            for (key, blob_idxs) in &hs {
                let mut uniq_blobs = blob_idxs.clone();
                uniq_blobs.sort();
                uniq_blobs.dedup();
                if uniq_blobs.len() > 1 {
                    let path_part = key.splitn(2, '/').collect::<Vec<&str>>()[1];
                    if !fuzz_list.contains(&String::from(path_part)) {
                        fuzz_list.push(String::from(path_part));
                    }
                }
            }
            fuzz_list.sort();
            
            let mut loc_total = 0;
            let mut source_files_count = 0;
            let mut loc_data = Vec::<(usize, String)>::new();
            for e in entries {
                let name = e.name.as_str();
                if name.ends_with(".rs") || name.ends_with(".swift") || name.ends_with(".c") || name.ends_with(".h") || name.ends_with(".mk") || name.ends_with(".sh") || name.ends_with("Makefile") {
                    if let Some(data) = read_file(name) {
                        source_files_count += 1;
                        let loc_count = count_loc(&data);
                        loc_total += loc_count;
                        loc_data.push((loc_count, String::from(name)));
                    }
                }
            }
            
            let mean_loc = if source_files_count > 0 {
                (loc_total as f64) / (source_files_count as f64)
            } else {
                0.0
            };
            
            loc_data.sort_by(|a, b| b.0.cmp(&a.0));
            let mut big_files_out = Vec::<String>::new();
            let mut seen_big = Vec::<String>::new();
            for &(count, ref path) in &loc_data {
                for t in &targets {
                    if path.starts_with(t) {
                        let mut sub_path = &path[t.len() + 1..];
                         if sub_path.starts_with("desk/") || sub_path.starts_with("metal/") || sub_path.starts_with("apple/") || sub_path.starts_with("droid/") {
                             let sub_parts: Vec<&str> = sub_path.split('/').collect();
                             sub_path = if sub_parts[0] == "desk" {
                                 &sub_path[5..]
                             } else {
                                 let skip_len = sub_parts[0].len() + sub_parts[1].len() + 2;
                                 &sub_path[skip_len..]
                             };
                         }
                        let key = alloc::format!("{} {}", count, sub_path);
                        if !seen_big.contains(&key) {
                            seen_big.push(key);
                            big_files_out.push(alloc::format!("  - {{ loc: {}, path: {} }}", count, sub_path));
                        }
                        break;
                    }
                }
                if big_files_out.len() >= 6 {
                    break;
                }
            }
            
            let mut rs_files_count = 0;
            let mut rs_unique_count = 0;
            let mut rscat = Vec::<u8>::new();
            let mut seen_rs_blobs = Vec::<u32>::new();
            for e in entries {
                if e.name.ends_with(".rs") && (e.name.contains("/libs/") || e.name.contains("/test/")) {
                    rs_files_count += 1;
                    if !seen_rs_blobs.contains(&e.blob) {
                        seen_rs_blobs.push(e.blob);
                        rs_unique_count += 1;
                        if let Some(data) = read_file(e.name.as_str()) {
                            rscat.extend_from_slice(&data);
                        }
                    }
                }
            }
            
            let src_raw_bytes = rscat.len();
            let src_gz_bytes = match deflate_bytes(&rscat) {
                Some(z) => z.len(),
                None => 0,
            };
            
            let dedup = if blobs.len() > 0 {
                (entries.len() as f64) / (blobs.len() as f64)
            } else {
                0.0
            };
            
            let mut yaml_lines = Vec::<String>::new();
            yaml_lines.push(alloc::format!("files: {}", entries.len()));
            yaml_lines.push(alloc::format!("blobs: {}", blobs.len()));
            yaml_lines.push(alloc::format!("dedup: {:.2}", dedup));
            yaml_lines.push(alloc::format!("bin_bytes: {}", out.len()));
            yaml_lines.push(alloc::format!("loc: {}", loc_total));
            yaml_lines.push(alloc::format!("source_files: {}", source_files_count));
            yaml_lines.push(alloc::format!("mean_loc: {:.1}", mean_loc));
            yaml_lines.push(alloc::format!("src_rs_files: {}", rs_files_count));
            yaml_lines.push(alloc::format!("src_rs_unique: {}", rs_unique_count));
            yaml_lines.push(alloc::format!("src_raw_bytes: {}", src_raw_bytes));
            yaml_lines.push(alloc::format!("src_gz_bytes: {}", src_gz_bytes));
            yaml_lines.push(String::from("cells:"));
            for c in cells_out {
                yaml_lines.push(c);
            }
            yaml_lines.push(String::from("fuzz:"));
            if fuzz_list.is_empty() {
                yaml_lines.push(String::from("  []"));
            } else {
                for f in &fuzz_list {
                    yaml_lines.push(alloc::format!("  - {}", f));
                }
            }
            yaml_lines.push(String::from("biggest:"));
            for b in big_files_out {
                yaml_lines.push(b);
            }
            
            let yaml_text = yaml_lines.join("\n") + "\n";
            let resolved_yaml = resolve_path("stats.yaml");
            let fd_yaml = unsafe { creat(cstr(&resolved_yaml).as_ptr() as *const c_char, 0o644) };
            if fd_yaml >= 0 {
                let mut yoff = 0usize;
                let ybytes = yaml_text.as_bytes();
                while yoff < ybytes.len() {
                    let n = unsafe { write(fd_yaml, ybytes[yoff..].as_ptr() as *const c_void, ybytes.len() - yoff) };
                    if n <= 0 { break; }
                    yoff += n as usize;
                }
                unsafe { close(fd_yaml) };
            }
            
            let stdout_line = alloc::format!("stats: {} {} {:.2} {} {} {} {} {}\n",
                entries.len(), blobs.len(), dedup, out.len(), src_gz_bytes, rs_unique_count, loc_total, fuzz_list.len());
            print_stdout(&stdout_line);
            
            if !fuzz_list.is_empty() {
                print_stdout(&alloc::format!("  fuzz: {}\n", fuzz_list.join(" ")));
            }
            
            for (cell_name, files_cnt, uniq_cnt) in cell_records {
                print_stdout(&alloc::format!("  {:<14} {}f {}u\n", cell_name, files_cnt, uniq_cnt));
            }
        }
    }
}
