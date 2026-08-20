import Foundation

func find(_ targetDir: String? = nil) -> [String] {
    let dir: String
    if let td = targetDir, !td.isEmpty {
        dir = td
    } else if let envDir = ProcessInfo.processInfo.environment["DIST_DIR"], !envDir.isEmpty {
        dir = envDir
    } else {
        let exe = (CommandLine.arguments[0] as NSString).resolvingSymlinksInPath
        let p = (exe as NSString).deletingLastPathComponent
        let fm = FileManager.default
        let candidates = [
            (p as NSString).appendingPathComponent("../../../dist"),
            (p as NSString).appendingPathComponent("../../dist"),
            (p as NSString).appendingPathComponent("dist"),
            "dist",
            "../../dist",
            "../../../dist"
        ]
        var found = "dist"
        for c in candidates {
            var isDir: ObjCBool = false
            if fm.fileExists(atPath: c, isDirectory: &isDir), isDir.boolValue {
                found = (c as NSString).standardizingPath
                break
            }
        }
        dir = found
    }

    guard let items = try? FileManager.default.contentsOfDirectory(atPath: dir) else { return [] }

    var files: [String] = []
    for item in items {
        if item == ".DS_Store" { continue }
        let p = (dir as NSString).appendingPathComponent(item)
        var isDir: ObjCBool = false
        if FileManager.default.fileExists(atPath: p, isDirectory: &isDir), !isDir.boolValue {
            files.append(p)
        }
    }
    return files
}
