use crate::pump::r#impl::r#impl;
use crate::r#type::{Address, Options, Payload};

#[no_mangle]
pub extern "C" fn Pump(address: Address, payload: Payload, options: Options) -> i32 {
    r#impl(address, payload, options)
}
