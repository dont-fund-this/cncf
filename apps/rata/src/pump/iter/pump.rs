use crate::r#type::{Address, Options, Payload};

pub fn iter_pump(address: Address, payload: Payload, options: Options) -> i32 {
    let defs = crate::defs::with::with();
    for def in defs {
        if (def.fit)(address, payload, options) {
            return (def.fun)(address, payload, options);
        }
    }
    -1
}
