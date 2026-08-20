import Foundation

func boot(_ targetDir: String? = nil) -> [Cabi] {
    var engines: [Cabi] = []
    if let envLib = ProcessInfo.processInfo.environment["PAT_LIB"], !envLib.isEmpty {
        if let c = bind(envLib) {
            engines.append(c)
            return engines
        }
    }

    let files = find(targetDir)
    for file in files {
        if let c = bind(file) {
            engines.append(c)
        }
    }
    return engines
}
