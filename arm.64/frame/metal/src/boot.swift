import Foundation

public func boot(targetDir: String? = nil) -> [Cabi] {
    var engines: [Cabi] = []

    if let envLib = ProcessInfo.processInfo.environment["PAT_LIB"], !envLib.isEmpty {
        if let bound = bind(binaryPath: envLib) {
            engines.append(bound)
            return engines
        }
    }

    let files = find(targetDir: targetDir)
    for file in files {
        if let bound = bind(binaryPath: file) {
            engines.append(bound)
        }
    }

    return engines
}
