use crate::defs::with::more as with_more;
use crate::r#type::Def;

#[no_mangle]
pub extern "C" fn More(def: Def) -> i32 {
    with_more(def)
}

pub fn more(def: Def) -> i32 {
    More(def)
}
