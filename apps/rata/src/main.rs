#![allow(dead_code)]
#![allow(unused_variables)]

mod defs;
pub mod impl_mod;
mod pump;
mod r#type;

use core::ffi::c_char;
use core::ffi::c_void;
use core::ffi::CStr;
use r#type::{Address, Def, Options, Payload};

extern "C" {
    pub fn More(def: Def) -> i32;
    pub fn Less(def: Def) -> i32;
    pub fn Pump(address: Address, payload: Payload, options: Options) -> i32;

    fn open(path: *const c_char, oflag: i32, ...) -> i32;
    fn close(fd: i32) -> i32;
    fn mmap(addr: *mut c_void, len: usize, prot: i32, flags: i32, fd: i32, offset: i64) -> *mut c_void;
}

const O_RDWR: i32 = 0x0002;
const O_SYNC: i32 = 0x0080;
const PROT_READ: i32 = 0x01;
const PROT_WRITE: i32 = 0x02;
const MAP_SHARED: i32 = 0x0001;
const MAP_FAILED: *mut c_void = !0 as *mut c_void;

#[inline]
fn rgb565(r: u8, g: u8, b: u8) -> u16 {
    ((r as u16 & 0xF8) << 8) | ((g as u16 & 0xFC) << 3) | (b as u16 >> 3)
}

fn ink() -> u16 {
    match std::env::var("PAT_COLOR").ok().as_deref() {
        Some("none") => rgb565(255, 255, 255),
        _ => rgb565(0, 255, 255),
    }
}

pub struct Fb {
    mem: *mut u16,
    w: usize,
    h: usize,
    back: Vec<u16>,
}

impl Fb {
    pub fn from_env() -> Option<Fb> {
        let s = std::env::var("PAT_FB").ok()?;
        let (addr, dims) = s.split_once(',')?;
        let addr = u64::from_str_radix(addr.trim().trim_start_matches("0x"), 16).ok()?;
        let (w, h) = dims.split_once('x')?;
        let w: usize = w.trim().parse().ok()?;
        let h: usize = h.trim().parse().ok()?;
        unsafe {
            let fd = open(b"/dev/mem\0".as_ptr() as *const c_char, O_RDWR | O_SYNC);
            if fd < 0 {
                return None;
            }
            let p = mmap(
                core::ptr::null_mut(),
                w * h * 2,
                PROT_READ | PROT_WRITE,
                MAP_SHARED,
                fd,
                addr as i64,
            );
            close(fd);
            if p == MAP_FAILED {
                return None;
            }
            Some(Fb { mem: p as *mut u16, w, h, back: vec![0u16; w * h] })
        }
    }

    pub fn draw(&mut self, vx: &[i32], hy: &[i32]) {
        let (w, h) = (self.w, self.h);
        if w == 0 || h == 0 {
            return;
        }
        let col = ink();
        let hw = (w.min(h) / 200).max(1);
        for p in self.back.iter_mut() {
            *p = 0;
        }
        for &x in vx {
            let cx = x.clamp(0, 100) as usize * (w - 1) / 100;
            let lo = cx.saturating_sub(hw);
            let hi = (cx + hw).min(w - 1);
            for y in 0..h {
                let row = y * w;
                for px in lo..=hi {
                    self.back[row + px] = col;
                }
            }
        }
        for &y in hy {
            let cy = y.clamp(0, 100) as usize * (h - 1) / 100;
            let lo = cy.saturating_sub(hw);
            let hi = (cy + hw).min(h - 1);
            for py in lo..=hi {
                let row = py * w;
                for px in 0..w {
                    self.back[row + px] = col;
                }
            }
        }
        unsafe { std::ptr::copy_nonoverlapping(self.back.as_ptr(), self.mem, w * h) };
    }
}

pub fn step(p: &mut i32, d: &mut i32) {
    *p += *d;
    if *p <= 0 || *p >= 100 {
        *d = -*d;
        *p = (*p).clamp(0, 100);
    }
}

pub fn scale(pct: i32, n: u16) -> u16 {
    (pct.clamp(0, 100) as u32 * n.saturating_sub(1) as u32 / 100) as u16
}

pub fn render_frame(cols: u16, rows: u16, vx: &[i32], hy: &[i32]) -> String {
    if cols == 0 || rows == 0 {
        return String::new();
    }
    let mut grid = vec![vec![' '; cols as usize]; rows as usize];

    for &x in vx {
        let cx = scale(x, cols) as usize;
        for r in 0..rows as usize {
            grid[r][cx] = '│';
        }
    }
    for &y in hy {
        let cy = scale(y, rows) as usize;
        for c in 0..cols as usize {
            let ch = if grid[cy][c] == '│' || grid[cy][c] == '┼' { '┼' } else { '─' };
            grid[cy][c] = ch;
        }
    }

    let label = "rata";
    let start_x = (cols as usize).saturating_sub(label.len()) / 2;
    let mid_y = rows as usize / 2;
    for (i, ch) in label.chars().enumerate() {
        if start_x + i < cols as usize {
            grid[mid_y][start_x + i] = ch;
        }
    }

    let mut out = String::new();
    for r in 0..rows as usize {
        out.push_str("\x1b[36m");
        for c in 0..cols as usize {
            out.push(grid[r][c]);
        }
        out.push_str("\x1b[0m\n");
    }
    out
}

pub static RATA_DRAW: Def = Def {
    sid: b"rata.draw\0".as_ptr() as *const core::ffi::c_char,
    tag: b"rata.draw\0".as_ptr() as *const core::ffi::c_char,
    fit: rata_draw_fit,
    fun: rata_draw_fun,
};

pub static RATA_STEP: Def = Def {
    sid: b"rata.step\0".as_ptr() as *const core::ffi::c_char,
    tag: b"rata.step\0".as_ptr() as *const core::ffi::c_char,
    fit: rata_step_fit,
    fun: rata_step_fun,
};

fn rata_draw_fit(address: Address, payload: Payload, options: Options) -> bool {
    if address.is_null() { return false; }
    let s = unsafe { CStr::from_ptr(address) }.to_bytes();
    s == b"rata.draw"
}

fn rata_draw_fun(address: Address, payload: Payload, options: Options) -> i32 {
    let f = render_frame(48, 24, &[20, 50, 78], &[30, 55, 80]);
    if let Some(mut fb) = Fb::from_env() {
        fb.draw(&[20, 50, 78], &[30, 55, 80]);
    }
    if let Some(into) = pump::into::fun::get_into(options) {
        let f_c = std::ffi::CString::new(f).unwrap_or_default();
        let opt_c = std::ffi::CString::new("once").unwrap_or_default();
        return unsafe { Pump(into, f_c.as_ptr(), opt_c.as_ptr()) };
    }
    1
}

fn rata_step_fit(address: Address, payload: Payload, options: Options) -> bool {
    if address.is_null() { return false; }
    let s = unsafe { CStr::from_ptr(address) }.to_bytes();
    s == b"rata.step"
}

fn rata_step_fun(address: Address, payload: Payload, options: Options) -> i32 {
    1
}

fn main() {
    for &def in impl_mod::all() {
        unsafe { More(def); }
    }

    println!("{{\n  \"app\": \"rata\",\n  \"status\": \"ready\",\n  \"defs\": {}\n}}", impl_mod::COUNT);
}
