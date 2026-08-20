use ratatui_core::buffer::{Buffer, Cell};
use ratatui_core::layout::Rect;
use ratatui_core::style::Color;

use crate::locale::Locale;

pub fn frame(cols: u16, rows: u16, locale: &Locale) -> Buffer {
    let mut buf = Buffer::empty(Rect::new(0, 0, cols, rows));
    for x in [20, 50, 78].map(|n| scale(n, cols)) {
        for y in 0..rows {
            let ch = if line(&buf, x, y) == '─' { '┼' } else { '│' };
            put(&mut buf, x, y, ch, Color::Magenta);
        }
    }
    for y in [30, 55, 80].map(|n| scale(n, rows)) {
        for x in 0..cols {
            let ch = if line(&buf, x, y) == '│' { '┼' } else { '─' };
            put(&mut buf, x, y, ch, Color::Magenta);
        }
    }
    center(&mut buf, rows.saturating_div(2).saturating_sub(1), &locale.alt, color(locale, 0));
    center(&mut buf, rows.saturating_div(2) + 1, &locale.bro, color(locale, 2));
    buf
}

pub fn ansi(buf: &Buffer) -> String {
    let mut out = String::new();
    for y in 0..buf.area.height {
        for x in 0..buf.area.width {
            if let Some(cell) = buf.cell((x, y)) {
                out.push_str("\x1b[0m");
                out.push_str(&sgr(cell));
                out.push_str(cell.symbol());
            }
        }
        out.push_str("\x1b[0m\n");
    }
    out
}

fn center(buf: &mut Buffer, y: u16, text: &str, color: Color) {
    let x = buf.area.width.saturating_sub(text.chars().count() as u16) / 2;
    for (i, ch) in text.chars().enumerate() {
        put(buf, x + i as u16, y, ch, color);
    }
}

fn put(buf: &mut Buffer, x: u16, y: u16, ch: char, color: Color) {
    if let Some(cell) = buf.cell_mut((x, y)) {
        cell.set_char(ch);
        cell.fg = color;
    }
}

fn line(buf: &Buffer, x: u16, y: u16) -> char {
    buf.cell((x, y)).and_then(|cell| cell.symbol().chars().next()).unwrap_or(' ')
}

fn scale(percent: u16, n: u16) -> u16 {
    (percent as u32)
        .saturating_mul(n.saturating_sub(1) as u32)
        .checked_div(100)
        .unwrap_or(0) as u16
}

fn color(locale: &Locale, index: usize) -> Color {
    locale.colors.get(index).copied().unwrap_or(Color::Magenta)
}

fn sgr(cell: &Cell) -> String {
    match cell.fg {
        Color::Rgb(r, g, b) => format!("\x1b[38;2;{};{};{}m", r, g, b),
        Color::Magenta => "\x1b[35m".into(),
        _ => String::new(),
    }
}
