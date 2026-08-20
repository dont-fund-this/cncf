import Foundation

struct BootDef {
    let cmdline: String
    let fbW: Int
    let fbH: Int
    let ram: Int
}

let bootDef = BootDef(
    cmdline: "console=hvc0 root=/dev/vda rw PAT_DEVICE=apple-watch-alpi",
    fbW: 396,
    fbH: 484,
    ram: 32
)

func runtimeRoot() -> String {
    FileManager.default.urls(for: .applicationSupportDirectory, in: .userDomainMask)[0]
        .appendingPathComponent("ta-in-patr-alpi").path
}

func prepareMounts() -> Bool {
    let files = FileManager.default
    let root = runtimeRoot()
    let data = root + "/9p/data"
    let exports = root + "/exports"
    try? files.createDirectory(atPath: data, withIntermediateDirectories: true)
    try? files.createDirectory(atPath: exports, withIntermediateDirectories: true)
    setenv("PAT_EFS_EXPORT_DIR", exports, 1)
    return true
}

func mountsJson() -> String {
    "[{\"tag\":\"data\",\"host\":\"\(runtimeRoot())/9p/data\",\"mode\":\"rw\"}]"
}
