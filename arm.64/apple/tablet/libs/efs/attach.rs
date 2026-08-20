use crate::abi::keep;
use crate::help::help;
use crate::version::version;
use crate::read::read;
use crate::load::load;
use crate::zip::zip;
use crate::txt::txt;
use crate::list::list;
use crate::size::size;
use crate::write::write;
use crate::peek::peek;
use crate::sniff::sniff;
use crate::export::export;
use crate::type_::InvokeFn;
use crate::with::more;

#[no_mangle]
pub unsafe extern "C" fn attach(cb: Option<InvokeFn>) -> bool {
    keep(cb);
    more(help());
    more(version());
    more(read());
    more(load());
    more(zip());
    more(txt());
    more(list());
    more(size());
    more(write());
    more(peek());
    more(sniff());
    more(export());
    true
}
