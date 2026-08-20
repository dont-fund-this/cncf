use crate::pump::many::many;
use crate::pump::none::none;
use crate::pump::once::once;
use crate::pump::want;
use crate::r#type::{Address, Options, Payload};

pub fn r#impl(address: Address, payload: Payload, options: Options) -> i32 {
    if want::none(options) {
        return none(address, payload, options);
    }
    if want::once(options) {
        return once(address, payload, options);
    }
    if want::many(options) {
        return many(address, payload, options);
    }
    -1
}
