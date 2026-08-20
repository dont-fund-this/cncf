pub mod api;
pub mod version;

use crate::r#type::{Def, Defs};

pub static ALL_DEFS: [Def; 3] = [
    version::get::VersionGet,
    api::v1::namespaces::post::NamespacePost,
    api::v1::namespaces::serviceaccounts::post::ServiceAccountPost,
];

pub static COUNT: usize = 3;

pub fn all() -> Defs {
    &ALL_DEFS
}
