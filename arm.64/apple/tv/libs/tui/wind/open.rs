use core::ffi::{c_char, c_void};
use core::mem::transmute;
use core::ptr::null_mut;

use super::sid::sid;
use super::fill::fill;
use super::type_::Rect;

const W: f64 = 640.0;
const H: f64 = 400.0;
const TITLED: u64 = 0x1;
const CLOSABLE: u64 = 0x2;
const BUFFERED: u64 = 0x2;
const REGULAR: i64 = 0;
const KEYDOWN: u64 = 10;

extern "C" {
    fn objc_getClass(name: *const c_char) -> *mut c_void;
    fn sel_registerName(name: *const c_char) -> *mut c_void;
    fn objc_msgSend();
}

type Msg = unsafe extern "C" fn(*mut c_void, *mut c_void) -> *mut c_void;
type MsgPtr = unsafe extern "C" fn(*mut c_void, *mut c_void, *mut c_void) -> *mut c_void;
type MsgInt = unsafe extern "C" fn(*mut c_void, *mut c_void, i64) -> *mut c_void;
type MsgFlag = unsafe extern "C" fn(*mut c_void, *mut c_void, bool) -> *mut c_void;
type MsgText = unsafe extern "C" fn(*mut c_void, *mut c_void, *const c_char) -> *mut c_void;
type MsgSeen = unsafe extern "C" fn(*mut c_void, *mut c_void) -> bool;
type MsgLong = unsafe extern "C" fn(*mut c_void, *mut c_void) -> u64;
type MsgMake = unsafe extern "C" fn(*mut c_void, *mut c_void, Rect, u64, u64, bool) -> *mut c_void;
type MsgPump = unsafe extern "C" fn(*mut c_void, *mut c_void, u64, *mut c_void, *mut c_void, bool) -> *mut c_void;

pub fn open() {
    unsafe {
        let msg: Msg = transmute(objc_msgSend as unsafe extern "C" fn());
        let msg_ptr: MsgPtr = transmute(objc_msgSend as unsafe extern "C" fn());
        let msg_int: MsgInt = transmute(objc_msgSend as unsafe extern "C" fn());
        let msg_flag: MsgFlag = transmute(objc_msgSend as unsafe extern "C" fn());
        let msg_text: MsgText = transmute(objc_msgSend as unsafe extern "C" fn());
        let msg_seen: MsgSeen = transmute(objc_msgSend as unsafe extern "C" fn());
        let msg_long: MsgLong = transmute(objc_msgSend as unsafe extern "C" fn());
        let msg_make: MsgMake = transmute(objc_msgSend as unsafe extern "C" fn());
        let msg_pump: MsgPump = transmute(objc_msgSend as unsafe extern "C" fn());

        let app = msg(
            objc_getClass(c"NSApplication".as_ptr()),
            sel_registerName(c"sharedApplication".as_ptr()),
        );
        if app.is_null() {
            return;
        }
        let pool = msg(
            msg(
                objc_getClass(c"NSAutoreleasePool".as_ptr()),
                sel_registerName(c"alloc".as_ptr()),
            ),
            sel_registerName(c"init".as_ptr()),
        );
        msg_int(app, sel_registerName(c"setActivationPolicy:".as_ptr()), REGULAR);
        let win = msg_make(
            msg(
                objc_getClass(c"NSWindow".as_ptr()),
                sel_registerName(c"alloc".as_ptr()),
            ),
            sel_registerName(c"initWithContentRect:styleMask:backing:defer:".as_ptr()),
            Rect { x: 200.0, y: 200.0, w: W, h: H },
            TITLED | CLOSABLE,
            BUFFERED,
            false,
        );
        if win.is_null() {
            msg(pool, sel_registerName(c"drain".as_ptr()));
            return;
        }
        msg_flag(win, sel_registerName(c"setReleasedWhenClosed:".as_ptr()), false);
        fill(win, W, H);
        let text = objc_getClass(c"NSString".as_ptr());
        let title = msg_text(
            text,
            sel_registerName(c"stringWithUTF8String:".as_ptr()),
            sid().as_ptr(),
        );
        msg_ptr(win, sel_registerName(c"setTitle:".as_ptr()), title);
        msg(win, sel_registerName(c"center".as_ptr()));
        msg(app, sel_registerName(c"finishLaunching".as_ptr()));
        msg_ptr(win, sel_registerName(c"makeKeyAndOrderFront:".as_ptr()), null_mut());
        msg_flag(app, sel_registerName(c"activateIgnoringOtherApps:".as_ptr()), true);
        let future = msg(
            objc_getClass(c"NSDate".as_ptr()),
            sel_registerName(c"distantFuture".as_ptr()),
        );
        let mode = msg_text(
            text,
            sel_registerName(c"stringWithUTF8String:".as_ptr()),
            c"kCFRunLoopDefaultMode".as_ptr(),
        );
        let next = sel_registerName(c"nextEventMatchingMask:untilDate:inMode:dequeue:".as_ptr());
        let send = sel_registerName(c"sendEvent:".as_ptr());
        let seen = sel_registerName(c"isVisible".as_ptr());
        let kind = sel_registerName(c"type".as_ptr());
        let chars = sel_registerName(c"charactersIgnoringModifiers".as_ptr());
        let utf8 = sel_registerName(c"UTF8String".as_ptr());
        loop {
            let event = msg_pump(app, next, u64::MAX, future, mode, true);
            if !event.is_null() {
                if msg_long(event, kind) == KEYDOWN {
                    let s = msg(event, chars);
                    if !s.is_null() {
                        let c = msg(s, utf8) as *const c_char;
                        if !c.is_null() && *c == b'q' as c_char {
                            break;
                        }
                    }
                }
                msg_ptr(app, send, event);
            }
            if !msg_seen(win, seen) {
                break;
            }
        }
        msg_ptr(win, sel_registerName(c"orderOut:".as_ptr()), null_mut());
        msg(win, sel_registerName(c"release".as_ptr()));
        msg(pool, sel_registerName(c"drain".as_ptr()));
    }
}
