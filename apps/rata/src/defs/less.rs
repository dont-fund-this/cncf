use crate::defs::with::less as with_less;
use crate::r#type::Def;

#[no_mangle]
pub extern "C" fn Less(def: Def) -> i32 {
    with_less(def)
}

pub fn less(def: Def) -> i32 {
    Less(def)
}
