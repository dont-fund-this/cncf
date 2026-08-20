use crate::abi::keep;
use crate::with::less;

#[no_mangle]
pub unsafe extern "C" fn detach() -> bool {
    less(c"tui.term".as_ptr());
    less(c"tui.out".as_ptr());
    less(c"tui.wind".as_ptr());
    less(c"tui.rect.take".as_ptr());
    less(c"tui.rect".as_ptr());
    less(c"tui.version".as_ptr());
    less(c"tui.help".as_ptr());
    keep(None);
    true
}
