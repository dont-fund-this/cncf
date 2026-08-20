use crate::r#type::{Def, Defs};
use crate::{RATA_DRAW, RATA_STEP};

pub const COUNT: usize = 2;

static DEFS: [Def; COUNT] = [RATA_DRAW, RATA_STEP];

pub fn all() -> Defs {
    &DEFS
}
