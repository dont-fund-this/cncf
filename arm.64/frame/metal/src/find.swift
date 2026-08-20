import Foundation

public func find(targetDir: String? = nil) -> [String] {
    var dir = targetDir
    if dir == nil || dir!.isEmpty {
        if let envDir = ProcessInfo.processInfo.environment["DIST_DIR"], !envDir.isEmpty {
            dir = envDir
        } else {
            let fm = FileManager.default
            let cwd = fm.currentDirectoryPath
            let candidates = [
                (cwd as NSString).appendingPathComponent("dist"),
                (cwd as NSString).appendingPathComponent("../../dist"),
                (cwd as NSString).appendingPathComponent("../../../dist")
            ]
            dir = candidates.first { fm.fileExists(atPath: $0) } ?? "dist"
        }
    }

    guard let d = dir, FileManager.default.fileExists(atPath: d) else {
        return []
    }

    let fm = FileManager.default
    guard let entries = try? fm.contentsOfDirectory(atPath: d) else {
        return []
    }

    return entries
        .filter { $0 != ".DS_Store" }
        .map { (d as NSString).appendingPathComponent($0) }
        .filter {
            var isDir: ObjCBool = false
            return fm.fileExists(atPath: $0, isDirectory: &isDir) && !isDir.boolValue
        }
}
