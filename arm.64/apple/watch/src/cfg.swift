import Foundation

func parseLocaleYaml(_ yaml: String) -> [String: String] {
    var dict = [String: String]()
    var colList = [String]()
    var inCol = false
    let lines = yaml.components(separatedBy: .newlines)
    for line in lines {
        let trimmed = line.trimmingCharacters(in: .whitespaces)
        if trimmed.isEmpty { continue }
        if trimmed.hasPrefix("pos:") {
            dict["pos"] = trimmed.dropFirst(4).trimmingCharacters(in: .whitespaces).trimmingCharacters(in: CharacterSet(charactersIn: "\"'"))
        } else if trimmed.hasPrefix("neg:") {
            dict["neg"] = trimmed.dropFirst(4).trimmingCharacters(in: .whitespaces).trimmingCharacters(in: CharacterSet(charactersIn: "\"'"))
        } else if trimmed.hasPrefix("neu:") {
            dict["neu"] = trimmed.dropFirst(4).trimmingCharacters(in: .whitespaces).trimmingCharacters(in: CharacterSet(charactersIn: "\"'"))
        } else if trimmed.hasPrefix("bro:") {
            dict["bro"] = trimmed.dropFirst(4).trimmingCharacters(in: .whitespaces).trimmingCharacters(in: CharacterSet(charactersIn: "\"'"))
        } else if trimmed.hasPrefix("alt:") {
            dict["alt"] = trimmed.dropFirst(4).trimmingCharacters(in: .whitespaces).trimmingCharacters(in: CharacterSet(charactersIn: "\"'"))
        } else if trimmed.hasPrefix("col:") {
            inCol = true
        } else if inCol && trimmed.hasPrefix("-") {
            let val = trimmed.dropFirst().trimmingCharacters(in: .whitespaces).trimmingCharacters(in: CharacterSet(charactersIn: "\"'"))
            colList.append(val)
        } else if inCol && !trimmed.hasPrefix("-") {
            inCol = false
        }
    }
    dict["col"] = colList.joined(separator: ",")
    return dict
}

func primaryCfg() -> String {
    let t = runtimeRoot() + "/ocis/riscv64/apple/watch/" + target() + "/"
    let files = FileManager.default
    let bios = files.fileExists(atPath: t + "bios64.bin") ? t + "bios64.bin" : t + "bbl64.bin"
    let kernel = files.fileExists(atPath: t + "risc64.bin") ? t + "risc64.bin" : (files.fileExists(atPath: t + "kernel.bin") ? t + "kernel.bin" : t + "kernel-riscv64.bin")
    let drive = t + "rootfs.ext2"
    let fbField = useFb ? ",\"fb\":\"\(bootDef.fbW)x\(bootDef.fbH)\"" : ""
    
    var extraCmdline = ""
    if let yaml = efsReadStr("locales/def/env.yaml") {
        let locale = parseLocaleYaml(yaml)
        if let alt = locale["alt"] { extraCmdline += " alt=\(alt)" }
        if let pos = locale["pos"] { extraCmdline += " pos=\(pos)" }
        if let neg = locale["neg"] { extraCmdline += " neg=\(neg)" }
        if let neu = locale["neu"] { extraCmdline += " neu=\(neu)" }
        if let bro = locale["bro"] { extraCmdline += " bro=\(bro)" }
        if let col = locale["col"] { extraCmdline += " col=\(col)" }
    }
    
    return "{\"bios\":\"\(bios)\",\"kernel\":\"\(kernel)\",\"drive\":\"\(drive)\",\"cmdline\":\"\(bootDef.cmdline)\(extraCmdline)\",\"ram\":\(bootDef.ram)\(fbField),\"mounts\":\(mountsJson())}"
}
