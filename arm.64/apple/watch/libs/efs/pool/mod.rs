use alloc::vec::Vec;

#[path = "type.rs"] mod type_;
mod hold;
mod data;
mod ready;
mod entries;
mod payload;
mod blob_method;
mod blob_crc;
mod blob_stored;
mod blob_orig;

pub use type_::{Blob, Entry};
pub use hold::hold;
pub use data::data;
pub use ready::ready;
pub use entries::entries;
pub use payload::payload;
pub use blob_method::blob_method;
pub use blob_crc::blob_crc;
pub use blob_stored::blob_stored;
pub use blob_orig::blob_orig;

static mut POOL: type_::Pool = type_::Pool {
    addr: core::ptr::null(),
    blobs: Vec::new(),
    entries: Vec::new(),
    ready: false,
};
