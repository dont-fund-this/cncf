import Foundation

struct BootDef {
    let cmdline: String
    let fbW: Int
    let fbH: Int
    let ram: Int
}

func efsReadStr(_ path: String) -> String? {
    guard let reply = post("efs.read", "{\"path\":\"\(path)\"}").first,
          let data = reply.data(using: .utf8),
          let object = try? JSONSerialization.jsonObject(with: data) as? [String: Any],
          let text = object["text"] as? String,
          let raw = Data(base64Encoded: text) else { return nil }
    return String(data: raw, encoding: .utf8)
}

func target() -> String {
    (Bundle.main.object(forInfoDictionaryKey: "PATTarget") as? String) ?? ""
}

func device() -> String {
    (Bundle.main.object(forInfoDictionaryKey: "PATDev") as? String) ?? ""
}

func boot() -> BootDef {
    var value = BootDef(
        cmdline: "console=hvc0 root=/dev/vda rw PAT_DEVICE=\(device())",
        fbW: 396,
        fbH: 484,
        ram: 64
    )
    guard let text = efsReadStr("box/boot.json"), let data = text.data(using: .utf8),
          let root = try? JSONSerialization.jsonObject(with: data) as? [String: Any] else { return value }
    let cmdline = root["cmdline"] as? String ?? value.cmdline
    guard let devices = root["devices"] as? [String: Any],
          let item = devices[device()] as? [String: Any] else {
        return BootDef(cmdline: cmdline, fbW: value.fbW, fbH: value.fbH, ram: value.ram)
    }
    let size = (item["fb"] as? String ?? "").split(separator: "x")
    let width = size.count == 2 ? Int(size[0]) ?? value.fbW : value.fbW
    let height = size.count == 2 ? Int(size[1]) ?? value.fbH : value.fbH
    let ram = item["ram"] as? Int ?? value.ram
    value = BootDef(cmdline: cmdline, fbW: width, fbH: height, ram: ram)
    return value
}

var bootDef = BootDef(
    cmdline: "console=hvc0 root=/dev/vda rw PAT_DEVICE=apple-watch-alpi",
    fbW: 396,
    fbH: 484,
    ram: 64
)

func runtimeRoot() -> String {
    let root = FileManager.default.urls(for: .applicationSupportDirectory, in: .userDomainMask)[0]
    return root.appendingPathComponent("ta-in-patr-" + target()).path
}

func prepareMounts() -> Bool {
    let files = FileManager.default
    let root = runtimeRoot()
    let data = root + "/9p/data"
    let exports = root + "/exports"
    let guest = root + "/ocis/riscv64/apple/watch/" + target()
    do {
        try files.createDirectory(atPath: data, withIntermediateDirectories: true)
        try files.createDirectory(atPath: exports, withIntermediateDirectories: true)
        try files.createDirectory(atPath: guest, withIntermediateDirectories: true)
    } catch { return false }
    setenv("PAT_EFS_EXPORT_DIR", exports, 1)
    let seed = data + "/types.csv"
    if !files.fileExists(atPath: seed) {
        if let b = Bundle.main.path(forResource: "types", ofType: "csv") {
            try? files.copyItem(atPath: b, toPath: seed)
        } else {
            _ = efsFetch("9p/data/types.csv", seed)
        }
    }
    let base = "ocis/riscv64/apple/watch/" + target()
    for (name, fallback) in [("bios64.bin", "bbl64.bin"), ("risc64.bin", "kernel-riscv64.bin"), ("rootfs.ext2", "rootfs.ext2")] {
        let dest = guest + "/" + name
        let nameWithoutExt = (name as NSString).deletingPathExtension
        let ext = (name as NSString).pathExtension
        if let bundlePath = Bundle.main.path(forResource: nameWithoutExt, ofType: ext) {
            try? files.removeItem(atPath: dest)
            try? files.copyItem(atPath: bundlePath, toPath: dest)
        } else if !files.fileExists(atPath: dest) {
            if !efsFetch(base + "/" + name, dest) {
                if !efsFetch(base + "/" + fallback, dest) { return false }
            }
        }
    }
    return true
}

func mountsJson() -> String {
    "[{\"tag\":\"data\",\"host\":\"\(runtimeRoot())/9p/data\",\"mode\":\"rw\"}]"
}
