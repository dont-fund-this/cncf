mod abi;
mod app;
mod bell;
mod compat;
mod defs;
mod fb;
mod fixtures;
mod glyph;
mod host;
mod invoke;
mod jam;
mod locale;
mod post;
mod render;
mod runtime;
mod strict;

fn main() -> std::io::Result<()> {
    if std::process::id() == 1 {
        fixtures::fixtures();
    }
    eprintln!("RATA-START {}", std::env::var("PAT_DEVICE").unwrap_or_default());
    let mut app = app::App::new();
    runtime::run(&mut app)
}
