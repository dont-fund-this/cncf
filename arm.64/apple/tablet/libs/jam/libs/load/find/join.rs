use alloc::string::String;

pub fn join(dir: &str, name: &str) -> String {
    let mut p: String = dir.into();
    p.push('/');
    p.push_str(name);
    p
}
