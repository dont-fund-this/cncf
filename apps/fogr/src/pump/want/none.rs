use crate::r#type::Options;

pub fn none(options: Options) -> bool {
    options.is_null()
}
