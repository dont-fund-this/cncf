use ratatui_core::style::Color;

pub struct Locale {
    pub alt: String,
    pub bro: String,
    pub colors: Vec<Color>,
}

impl Locale {
    pub fn load() -> Self {
        let alt = std::env::var("alt").unwrap_or_else(|_| "ta-in-patr".into());
        let bro = std::env::var("bro").unwrap_or_default();
        let colors = std::env::var("col")
            .unwrap_or_default()
            .split(',')
            .filter_map(color)
            .collect();
        Self { alt, bro, colors }
    }
}

fn color(text: &str) -> Option<Color> {
    let hex = text.trim().strip_prefix('#')?;
    if hex.len() != 6 {
        return None;
    }
    Some(Color::Rgb(
        u8::from_str_radix(&hex[0..2], 16).ok()?,
        u8::from_str_radix(&hex[2..4], 16).ok()?,
        u8::from_str_radix(&hex[4..6], 16).ok()?,
    ))
}
