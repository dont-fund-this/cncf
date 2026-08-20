use alloc::string::String;

mod set;
mod get;

pub use set::set;
pub use get::get;

static mut TEXT: String = String::new();
