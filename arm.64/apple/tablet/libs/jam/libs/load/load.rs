pub fn load() -> bool {
    super::unload::unload();
    let paths = super::find::find();
    for sig in ["box", "efs", "tui"] {
        let name = alloc::format!("lib{}", sig);
        let mut found = None;
        for path in &paths {
            if path.rsplit('/').next() == Some(name.as_str()) {
                if found.is_some() {
                    super::unload::unload();
                    return false;
                }
                found = Some(path.as_str());
            }
        }
        let path = match found {
            Some(path) => path,
            None => {
                super::unload::unload();
                return false;
            }
        };
        if !matches!(super::one::one(path, sig), super::one::Outcome::Attached) {
            super::unload::unload();
            return false;
        }
    }
    true
}
