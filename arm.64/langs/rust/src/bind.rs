use crate::r#type::{Cabi, LessFn, MoreFn, PumpFn};
use libloading::{Library, Symbol};
use std::path::Path;

pub fn bind(binary_path: &str) -> Option<Cabi> {
    let filename = Path::new(binary_path).file_name()?.to_str()?;
    if matches!(filename, "c" | "cpp" | "rust" | "go" | "swift" | "haskell" | "zig" | "v" | "slint_sample") {
        return None;
    }

    unsafe {
        let lib = Library::new(binary_path).ok()?;
        let more: Option<MoreFn> = lib.get(b"More\0").ok().map(|s: Symbol<MoreFn>| *s);
        let pump: Symbol<PumpFn> = lib.get(b"Pump\0").ok()?;
        let pump_fn: PumpFn = *pump;
        let less: Option<LessFn> = lib.get(b"Less\0").ok().map(|s: Symbol<LessFn>| *s);

        Some(Cabi {
            name: filename.to_string(),
            path: binary_path.to_string(),
            lib,
            more,
            pump: pump_fn,
            less,
        })
    }
}
