use crate::abi::keep;
use crate::with::less;

#[no_mangle]
pub unsafe extern "C" fn detach() -> bool {
    less(c"efs.export".as_ptr());
    less(c"efs.sniff".as_ptr());
    less(c"efs.peek".as_ptr());
    less(c"efs.write".as_ptr());
    less(c"efs.size".as_ptr());
    less(c"efs.list".as_ptr());
    less(c"efs.bin.txt".as_ptr());
    less(c"efs.zip".as_ptr());
    less(c"efs.load".as_ptr());
    less(c"efs.read".as_ptr());
    less(c"efs.version".as_ptr());
    less(c"efs.help".as_ptr());
    keep(None);
    true
}
