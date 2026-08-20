use std::io;

use crate::app::App;

pub fn run(app: &mut App) -> io::Result<()> {
    let _ = crate::jam::prep();
    crate::host::install(app);
    if let Some(inventory) = crate::post::post("jam.list", "{}", "once").first() {
        eprintln!("JAM-LIBS {}", inventory);
    }
    if let Some(exported) = crate::post::post(
        "efs.export", "{\"path\":\"code/code.zip\"}", "once",
    ).first() {
        eprintln!("CODE-ZIP {}", exported);
    }
    let mut proved = false;
    while app.running() {
        let _ = app.draw();
        let replies = crate::post::post("tui.rect", "{}", "once");
        if !proved {
            if let Some(reply) = replies.first() {
                eprintln!(
                    "FRAME-OK {} {}",
                    std::env::var("alt").unwrap_or_default(),
                    reply,
                );
                proved = true;
            }
        }
        if !wait()? {
            app.stop();
        }
    }
    crate::host::uninstall();
    crate::jam::detach();
    Ok(())
}

fn wait() -> io::Result<bool> {
    let mut fd = libc::pollfd { fd: 0, events: libc::POLLIN, revents: 0 };
    let selected = unsafe { libc::poll(&mut fd, 1, 16) };
    if selected < 0 {
        return Err(io::Error::last_os_error());
    }
    if selected == 0 || fd.revents & libc::POLLIN == 0 {
        return Ok(true);
    }
    let mut byte = 0u8;
    let read = unsafe { libc::read(0, &mut byte as *mut u8 as *mut libc::c_void, 1) };
    Ok(read > 0 && byte != b'q')
}
