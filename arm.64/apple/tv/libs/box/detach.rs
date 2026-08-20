use crate::abi::keep;
use crate::with::less;

#[no_mangle]
pub unsafe extern "C" fn detach() -> bool {
    less(c"box.stop".as_ptr());
    less(c"box.fb".as_ptr());
    less(c"box.poll".as_ptr());
    less(c"box.send".as_ptr());
    less(c"box.start".as_ptr());
    less(c"box.list".as_ptr());
    less(c"box.prep".as_ptr());
    less(c"box.version".as_ptr());
    less(c"box.help".as_ptr());
    keep(None);
    true
}
