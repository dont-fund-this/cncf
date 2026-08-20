pub fn makedev(major: u32, minor: u32) -> i32 {
    (((major & 0xff) << 24) | (minor & 0xffffff)) as i32
}
