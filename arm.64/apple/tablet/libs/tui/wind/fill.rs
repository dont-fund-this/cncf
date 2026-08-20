use core::ffi::{c_char, c_void};
use core::mem::transmute;

use super::type_::Rect;

const CENTER: i64 = 2;

extern "C" {
    fn objc_getClass(name: *const c_char) -> *mut c_void;
    fn sel_registerName(name: *const c_char) -> *mut c_void;
    fn objc_msgSend();
}

type Msg = unsafe extern "C" fn(*mut c_void, *mut c_void) -> *mut c_void;
type MsgPtr = unsafe extern "C" fn(*mut c_void, *mut c_void, *mut c_void) -> *mut c_void;
type MsgInt = unsafe extern "C" fn(*mut c_void, *mut c_void, i64) -> *mut c_void;
type MsgText = unsafe extern "C" fn(*mut c_void, *mut c_void, *const c_char) -> *mut c_void;
type MsgFont = unsafe extern "C" fn(*mut c_void, *mut c_void, f64, f64) -> *mut c_void;
type MsgRect = unsafe extern "C" fn(*mut c_void, *mut c_void, Rect) -> *mut c_void;

pub fn fill(win: *mut c_void, w: f64, h: f64) {
    unsafe {
        let msg: Msg = transmute(objc_msgSend as unsafe extern "C" fn());
        let msg_ptr: MsgPtr = transmute(objc_msgSend as unsafe extern "C" fn());
        let msg_int: MsgInt = transmute(objc_msgSend as unsafe extern "C" fn());
        let msg_text: MsgText = transmute(objc_msgSend as unsafe extern "C" fn());
        let msg_font: MsgFont = transmute(objc_msgSend as unsafe extern "C" fn());
        let msg_rect: MsgRect = transmute(objc_msgSend as unsafe extern "C" fn());

        let color = objc_getClass(c"NSColor".as_ptr());
        msg_ptr(
            win,
            sel_registerName(c"setBackgroundColor:".as_ptr()),
            msg(color, sel_registerName(c"blackColor".as_ptr())),
        );
        let content = msg(win, sel_registerName(c"contentView".as_ptr()));
        if content.is_null() {
            return;
        }
        let text = objc_getClass(c"NSString".as_ptr());
        let field = objc_getClass(c"NSTextField".as_ptr());
        let fonts = objc_getClass(c"NSFont".as_ptr());
        let with = sel_registerName(c"stringWithUTF8String:".as_ptr());
        let label = sel_registerName(c"labelWithString:".as_ptr());
        let mono = sel_registerName(c"monospacedSystemFontOfSize:weight:".as_ptr());
        let set_font = sel_registerName(c"setFont:".as_ptr());
        let set_color = sel_registerName(c"setTextColor:".as_ptr());
        let set_align = sel_registerName(c"setAlignment:".as_ptr());
        let set_frame = sel_registerName(c"setFrame:".as_ptr());
        let attach = sel_registerName(c"addSubview:".as_ptr());
        let white = msg(color, sel_registerName(c"whiteColor".as_ptr()));

        let hello = msg_ptr(field, label, msg_text(text, with, c"hello".as_ptr()));
        msg_ptr(hello, set_font, msg_font(fonts, mono, 24.0, 0.0));
        msg_ptr(hello, set_color, white);
        msg_int(hello, set_align, CENTER);
        msg_rect(hello, set_frame, Rect { x: 0.0, y: h / 2.0 - 18.0, w, h: 36.0 });
        msg_ptr(content, attach, hello);

        let quit = msg_ptr(field, label, msg_text(text, with, c"q quit".as_ptr()));
        msg_ptr(quit, set_font, msg_font(fonts, mono, 16.0, 0.0));
        msg_ptr(quit, set_color, white);
        msg_int(quit, set_align, CENTER);
        msg_rect(quit, set_frame, Rect { x: 0.0, y: h / 2.0 - 90.0, w, h: 24.0 });
        msg_ptr(content, attach, quit);
    }
}
