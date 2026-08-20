use std::io::{self, Write};

use crate::bell::Bell;
use crate::fb::Fb;
use crate::locale::Locale;

pub struct App {
    fb: Option<Fb>,
    bell: Option<Bell>,
    locale: Locale,
    live: bool,
}

impl App {
    pub fn new() -> Self {
        Self { fb: Fb::open(), bell: Bell::open(), locale: Locale::load(), live: true }
    }

    pub fn draw(&mut self) -> io::Result<u32> {
        let (cols, rows) = self.dimensions();
        let frame = crate::render::frame(cols, rows, &self.locale);
        if let Some(fb) = &mut self.fb {
            fb.draw(&frame);
        } else {
            let mut out = io::stdout();
            out.write_all(format!("\x1b[H{}", crate::render::ansi(&frame)).as_bytes())?;
            out.flush()?;
        }
        Ok(self.bell.as_mut().map(Bell::ring).unwrap_or(0))
    }

    pub fn start(&mut self) {
        self.live = true;
    }

    pub fn stop(&mut self) {
        self.live = false;
    }

    pub fn running(&self) -> bool {
        self.live
    }

    pub fn view(&self) -> Option<(usize, usize, usize)> {
        self.fb.as_ref().map(Fb::view)
    }

    fn dimensions(&self) -> (u16, u16) {
        if let Some(fb) = &self.fb {
            return ((fb.w / 8).max(1) as u16, (fb.h / 20).max(1) as u16);
        }
        std::env::var("PAT_SHOT").ok().and_then(|value| {
            let (cols, rows) = value.split_once('x')?;
            Some((cols.parse().ok()?, rows.parse().ok()?))
        }).unwrap_or((48, 24))
    }
}
