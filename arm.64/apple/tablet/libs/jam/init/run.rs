pub fn run() -> Option<usize> {
    if crate::libs::load::load() {
        Some(crate::libs::load::libs().len())
    } else {
        None
    }
}
