use std::env;
use std::fs;
use std::path::{Path, PathBuf};

pub fn find(target_dir: Option<&str>) -> Vec<String> {
    let dir = if let Some(d) = target_dir {
        PathBuf::from(d)
    } else if let Ok(env_dir) = env::var("DIST_DIR") {
        PathBuf::from(env_dir)
    } else if let Ok(exe) = env::current_exe() {
        let p = exe.parent().unwrap_or(Path::new("."));
        if p.file_name().and_then(|n| n.to_str()) == Some("dist") && p.exists() {
            p.to_path_buf()
        } else if p.join("../../dist").exists() {
            p.join("../../dist").canonicalize().unwrap_or_else(|_| p.join("../../dist"))
        } else if p.join("../../../dist").exists() {
            p.join("../../../dist").canonicalize().unwrap_or_else(|_| p.join("../../../dist"))
        } else if Path::new("dist").exists() {
            PathBuf::from("dist").canonicalize().unwrap_or_else(|_| PathBuf::from("dist"))
        } else {
            PathBuf::from("dist")
        }
    } else {
        PathBuf::from("dist")
    };

    if !dir.exists() {
        return Vec::new();
    }

    let mut files = Vec::new();
    if let Ok(entries) = fs::read_dir(&dir) {
        for entry in entries.flatten() {
            let path = entry.path();
            if path.is_file() {
                if let Some(name) = path.file_name().and_then(|n| n.to_str()) {
                    if name != ".DS_Store" {
                        files.push(path.to_string_lossy().into_owned());
                    }
                }
            }
        }
    }
    files
}
