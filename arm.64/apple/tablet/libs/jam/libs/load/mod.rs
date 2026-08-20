use alloc::vec::Vec;
use crate::type_::Abi;

mod find;
mod bind;
mod cstr;
mod libs;
mod load;
mod one;
mod unload;

pub use self::libs::libs;
pub use self::load::load;
pub use self::unload::unload;

static mut LIBS: Vec<Abi> = Vec::new();
