mod hold;
mod setup;
mod free;

pub const ADDR: u64 = 0x300000000;

pub use hold::{H, PTR, W};
pub use setup::setup;
pub use free::free;
