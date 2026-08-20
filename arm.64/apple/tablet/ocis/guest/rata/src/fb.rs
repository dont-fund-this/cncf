use ratatui_core::buffer::Buffer;
use ratatui_core::style::Color;

pub struct Fb {
    mem: *mut u16,
    pub w: usize,
    pub h: usize,
}

impl Fb {
    pub fn open() -> Option<Self> {
        let text = std::env::var("PAT_FB").ok()?;
        let (address, dims) = text.split_once(',')?;
        let address = u64::from_str_radix(address.trim().trim_start_matches("0x"), 16).ok()?;
        let (w, h) = dims.split_once('x')?;
        let w = w.parse().ok()?;
        let h = h.parse().ok()?;
        let fd = unsafe { libc::open(c"/dev/mem".as_ptr(), libc::O_RDWR | libc::O_SYNC) };
        if fd < 0 {
            return None;
        }
        let mem = unsafe {
            libc::mmap(
                std::ptr::null_mut(),
                w * h * 2,
                libc::PROT_READ | libc::PROT_WRITE,
                libc::MAP_SHARED,
                fd,
                address as libc::off_t,
            )
        };
        unsafe { libc::close(fd) };
        if mem == libc::MAP_FAILED {
            return None;
        }
        Some(Self { mem: mem as *mut u16, w, h })
    }

    pub fn draw(&mut self, buf: &Buffer) {
        let pixels = unsafe { std::slice::from_raw_parts_mut(self.mem, self.w * self.h) };
        pixels.fill(0);
        let cols = buf.area.width as usize;
        let rows = buf.area.height as usize;
        let ox = self.w.saturating_sub(cols * 8) / 2;
        let oy = self.h.saturating_sub(rows * 20) / 2;
        for y in 0..rows {
            for x in 0..cols {
                if let Some(cell) = buf.cell((x as u16, y as u16)) {
                    let ink = match cell.fg {
                        Color::Rgb(r, g, b) => rgb565(r, g, b),
                        _ => rgb565(255, 0, 255),
                    };
                    crate::glyph::draw(
                        pixels,
                        self.w,
                        self.h,
                        ox + x * 8,
                        oy + y * 20,
                        cell.symbol().chars().next().unwrap_or(' '),
                        ink,
                    );
                }
            }
        }
    }

    pub fn view(&self) -> (usize, usize, usize) {
        (self.mem as usize, self.w, self.h)
    }
}

fn rgb565(r: u8, g: u8, b: u8) -> u16 {
    ((r as u16 & 0xf8) << 8) | ((g as u16 & 0xfc) << 3) | (b as u16 >> 3)
}
