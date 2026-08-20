use crate::type_::Def;
use super::sid::sid;
use super::tag::tag;
use super::fun::fun;
use super::fit::fit;

pub fn write() -> Def {
    Def {
        sid: sid().as_ptr(),
        tag: tag().as_ptr(),
        fun,
        fit,
    }
}
