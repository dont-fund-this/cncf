use crate::host::keep;
use crate::with::less;

#[no_mangle]
pub unsafe extern "C" fn detach() -> bool {
    crate::libs::load::unload();
    less(c"jam.help".as_ptr());
    less(c"jam.version".as_ptr());
    less(c"jam.init".as_ptr());
    less(c"jam.list".as_ptr());
    keep(None);
    true
}
