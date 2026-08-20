use alloc::string::String;

pub fn cstr(s: &str) -> String {
    let mut c: String = s.into();
    c.push('\0');
    c
}
