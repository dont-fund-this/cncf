use crate::abi::keep;
use crate::help::help;
use crate::version::version;
use crate::rect::rect;
use crate::take::take;
use crate::wind::wind;
use crate::out::out;
use crate::term::term;
use crate::type_::InvokeFn;
use crate::with::more;

#[no_mangle]
pub unsafe extern "C" fn attach(cb: Option<InvokeFn>) -> bool {
    keep(cb);
    more(help());
    more(version());
    more(rect());
    more(take());
    more(wind());
    more(out());
    more(term());
    true
}
