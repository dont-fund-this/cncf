use std::ffi::{c_char, CStr, CString};

use crate::abi::Def;
use crate::app::App;

pub fn install(app: &mut App) {
    let app = app as *mut App;
    crate::defs::more(Def {
        sid: CString::new("rata").unwrap(),
        tag: CString::new("rata.box.prep,rata.box.list,rata.box.start,rata.box.stop,rata.box.send,rata.box.poll,rata.box.fb,rata.gui.wind,rata.gui.term,rata.gui.capture,rata.gui.center,rata.gui.live,rata.gui.frame,rata.tui.rect,rata.tui.rect.take,rata.tui.wind,rata.tui.out,rata.tui.term").unwrap(),
        fit: Box::new(|address, payload, options| unsafe { fit(address, payload, options) }),
        fun: Box::new(move |address, payload, options| unsafe {
            fun(app, address, payload, options)
        }),
    });
}

pub fn uninstall() {
    crate::defs::less(c"rata");
}

unsafe fn fit(address: *const c_char, _payload: *const c_char, _options: *const c_char) -> bool {
    if address.is_null() {
        return false;
    }
    let address = unsafe { CStr::from_ptr(address) };
    address == c"rata" || c"rata.box.prep,rata.box.list,rata.box.start,rata.box.stop,rata.box.send,rata.box.poll,rata.box.fb,rata.gui.wind,rata.gui.term,rata.gui.capture,rata.gui.center,rata.gui.live,rata.gui.frame,rata.tui.rect,rata.tui.rect.take,rata.tui.wind,rata.tui.out,rata.tui.term"
        .to_string_lossy().split(',').any(|tag| tag.trim().as_bytes() == address.to_bytes())
}

unsafe fn fun(app: *mut App, address: *const c_char, _payload: *const c_char, options: *const c_char) {
    let app = unsafe { app.as_mut() };
    let address = unsafe { CStr::from_ptr(address) }.to_string_lossy();
    let Some(app) = app else { return };
    let payload = match address.as_ref() {
        "rata.box.start" => { app.start(); String::from("{\"ok\":true}") }
        "rata.box.stop" => { app.stop(); String::from("{\"ok\":true}") }
        "rata.box.list" => format!("{{\"running\":{}}}", app.running()),
        "rata.box.poll" => format!("{{\"running\":{}}}", app.running()),
        "rata.box.fb" => app.view().map(|(ptr, w, h)| {
            format!("{{\"ok\":true,\"ptr\":{},\"w\":{},\"h\":{}}}", ptr, w, h)
        }).unwrap_or_else(|| String::from("{\"ok\":false}")),
        "rata.box.prep" | "rata.box.send" => String::from("{\"ok\":true}"),
        _ => match app.draw() {
            Ok(generation) => format!(
                "{{\"ok\":true,\"fb\":{},\"generation\":{}}}",
                app.view().is_some(),
                generation,
            ),
            Err(_) => String::from("{\"ok\":false}"),
        },
    };
    reply(options, &payload);
}

fn reply(options: *const c_char, payload: &str) {
    if options.is_null() {
        return;
    }
    let Some(into) = crate::post::text(options, "into") else { return };
    let (Ok(address), Ok(payload)) = (CString::new(into), CString::new(payload)) else { return };
    crate::jam::invoke(&address, &payload, c"{\"strict\":\"once\"}");
}
