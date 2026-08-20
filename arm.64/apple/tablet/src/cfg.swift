import Foundation

func primaryCfg() -> String {
    let base = Bundle.main.bundlePath
    let bios = "\(base)/bios64.bin"
    let kernel = "\(base)/risc64.bin"
    let drive = "\(base)/rootfs.ext2"
    return "{\"bios\":\"\(bios)\",\"kernel\":\"\(kernel)\",\"drive\":\"\(drive)\",\"cmdline\":\"\(bootDef.cmdline)\",\"ram\":\(bootDef.ram),\"fb\":\"\(bootDef.fbW)x\(bootDef.fbH)\",\"mounts\":\(mountsJson())}"
}
