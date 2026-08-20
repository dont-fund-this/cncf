use crate::abi::keep;
use crate::help::help;
use crate::list::list;
use crate::poll::poll;
use crate::prep::prep;
use crate::send::send;
use crate::start::start;
use crate::stop::stop;
use crate::fbv::fbv;
use crate::type_::InvokeFn;
use crate::version::version;
use crate::with::more;

#[no_mangle]
pub unsafe extern "C" fn attach(cb: Option<InvokeFn>) -> bool {
    keep(cb);
    more(help());
    more(version());
    more(prep());
    more(list());
    more(start());
    more(send());
    more(poll());
    more(fbv());
    more(stop());
    true
}
