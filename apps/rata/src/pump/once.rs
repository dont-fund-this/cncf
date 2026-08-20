use crate::defs::with::with;
use crate::r#type::{Address, Options, Payload};

pub fn once(address: Address, payload: Payload, options: Options) -> i32 {
    for def in with() {
        if (def.fit)(address, payload, options) {
            return (def.fun)(address, payload, options);
        }
    }
    -1
}
