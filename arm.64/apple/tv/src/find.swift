import Foundation

func find() -> [String] {
    guard let base = Bundle.main.privateFrameworksPath,
          let names = try? FileManager.default.contentsOfDirectory(atPath: base) else { return [] }
    return names.sorted().compactMap { name in
        guard name.hasSuffix(".framework") else { return nil }
        let binary = (name as NSString).deletingPathExtension
        let path = "\(base)/\(name)/\(binary)"
        return FileManager.default.isExecutableFile(atPath: path) ? path : nil
    }
}
