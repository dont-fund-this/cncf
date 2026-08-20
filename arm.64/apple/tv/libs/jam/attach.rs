use crate::help::help;
use crate::host::keep;
use crate::init::init;
use crate::list::list;
use crate::type_::InvokeFn;
use crate::version::version;
use crate::with::{less, more};

#[no_mangle]
pub unsafe extern "C" fn attach(cb: Option<InvokeFn>) -> bool {
    keep(cb);
    more(help());
    more(version());
    more(init());
    more(list());
    if crate::libs::load::load() {
        return true;
    }
    less(c"jam.help".as_ptr());
    less(c"jam.version".as_ptr());
    less(c"jam.init".as_ptr());
    less(c"jam.list".as_ptr());
    keep(None);
    false
}
