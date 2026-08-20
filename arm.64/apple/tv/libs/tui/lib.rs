#![no_std]

extern crate alloc;

#[path = "type.rs"]
pub mod type_;

pub mod abi;

pub mod attach;
pub mod detach;
pub mod invoke;
pub mod report;
pub mod fall;
mod heap;
mod unb64;
mod hold;
mod with;
mod help;
mod version;
mod rect;
mod take;
mod wind;
mod out;
mod term;

pub use type_::InvokeFn;
