use super::abi::*;
use super::super::dev::*;

pub fn stat_to_qid(qid: *mut FSQID, st: *const Stat) {
    let mode = unsafe { (*st).st_mode } & S_IFMT;
    let ty = if mode == S_IFDIR {
        P9_QTDIR
    } else if mode == S_IFLNK {
        P9_QTSYMLINK
    } else {
        P9_QTFILE
    };
    unsafe {
        (*qid).type_ = ty;
        (*qid).version = 0;
        (*qid).path = (*st).st_ino;
    }
}
