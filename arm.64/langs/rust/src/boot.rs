use crate::bind::bind;
use crate::find::find;
use crate::r#type::Cabi;
use std::env;

pub fn boot(target_dir: Option<&str>) -> Vec<Cabi> {
    let mut engines = Vec::new();
    if let Ok(env_lib) = env::var("PAT_LIB") {
        if !env_lib.is_empty() {
            if let Some(c) = bind(&env_lib) {
                engines.push(c);
                return engines;
            }
        }
    }

    let files = find(target_dir);
    for file in files {
        if let Some(c) = bind(&file) {
            engines.push(c);
        }
    }
    engines
}
