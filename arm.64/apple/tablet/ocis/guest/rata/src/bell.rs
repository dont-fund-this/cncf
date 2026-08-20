pub struct Bell {
    mem: *mut u32,
    n: u32,
}

impl Bell {
    pub fn open() -> Option<Self> {
        let text = std::env::var("PAT_DOORBELL").ok()?;
        let address = u64::from_str_radix(text.trim().trim_start_matches("0x"), 16).ok()?;
        let mem = unsafe { map(address, 4096)? } as *mut u32;
        Some(Self { mem, n: 0 })
    }

    pub fn ring(&mut self) -> u32 {
        self.n = self.n.wrapping_add(1);
        unsafe { std::ptr::write_volatile(self.mem, self.n) };
        self.n
    }
}

unsafe fn map(address: u64, len: usize) -> Option<*mut libc::c_void> {
    let fd = unsafe { libc::open(c"/dev/mem".as_ptr(), libc::O_RDWR | libc::O_SYNC) };
    if fd < 0 {
        return None;
    }
    let mem = unsafe {
        libc::mmap(
            std::ptr::null_mut(),
            len,
            libc::PROT_READ | libc::PROT_WRITE,
            libc::MAP_SHARED,
            fd,
            address as libc::off_t,
        )
    };
    unsafe { libc::close(fd) };
    (mem != libc::MAP_FAILED).then_some(mem)
}
