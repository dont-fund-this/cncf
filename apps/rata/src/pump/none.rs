use crate::r#type::{Address, Options, Payload};

pub fn none(address: Address, payload: Payload, options: Options) -> i32 {
    let _ = (address, payload, options);
    -1
}
