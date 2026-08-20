use crate::defs::with::with;
use crate::r#type::{Address, Options, Payload};

pub fn many(address: Address, payload: Payload, options: Options) -> i32 {
    let mut count = 0;
    for def in with() {
        if (def.fit)(address, payload, options) {
            (def.fun)(address, payload, options);
            count += 1;
        }
    }
    count
}
