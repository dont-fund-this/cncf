pub fn unload() {
    unsafe {
        let libs = &mut *core::ptr::addr_of_mut!(super::LIBS);
        for abi in libs.drain(..) {
            (abi.detach)();
            super::bind::close(abi);
        }
    }
}
