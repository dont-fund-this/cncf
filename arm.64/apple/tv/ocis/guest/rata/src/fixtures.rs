use std::ffi::CString;

pub fn fixtures() {
    mount("proc", "/proc", "proc", "");
    mount("sysfs", "/sys", "sysfs", "");
    if let Ok(cmdline) = std::fs::read_to_string("/proc/cmdline") {
        for arg in cmdline.split_whitespace() {
            let Some((key, value)) = arg.split_once('=') else { continue };
            if key.starts_with("PAT_") || matches!(key, "alt" | "col" | "pos" | "neg" | "neu" | "bro") {
                std::env::set_var(key, value.trim_matches(|ch| ch == '"' || ch == '\''));
            }
        }
    }
    std::fs::create_dir_all("/mnt/data").ok();
    if mount("data", "/mnt/data", "9p", "trans=virtio,version=9p2000.L") {
        eprintln!("9P-OK /mnt/data");
    }
    let kind = std::env::var("PAT_DEVICE").unwrap_or_else(|_| "pid1".into());
    let path = String::from("/mnt/data/rata.txt");
    if std::fs::write(&path, "OK\n").is_ok() {
        eprintln!("WROTE {}", path);
    }
    if std::fs::read_link("/sbin/init").ok().as_deref() == Some(std::path::Path::new("/bin/rata")) {
        eprintln!("PID1-OK /sbin/init=/bin/rata");
    }
    eprintln!("PROFILE-OK {} Rata", kind);
}

fn mount(source: &str, target: &str, kind: &str, options: &str) -> bool {
    std::fs::create_dir_all(target).ok();
    let (Ok(source), Ok(target), Ok(kind), Ok(options)) = (
        CString::new(source), CString::new(target), CString::new(kind), CString::new(options),
    ) else { return false };
    unsafe {
        libc::mount(
            source.as_ptr(), target.as_ptr(), kind.as_ptr(), 0,
            options.as_ptr() as *const libc::c_void,
        ) == 0
    }
}
