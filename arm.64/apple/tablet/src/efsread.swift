import Foundation

func efsFetch(_ key: String, _ dest: String) -> Bool {
    if FileManager.default.fileExists(atPath: dest) { return true }
    let replies = post("efs.read", "{\"path\":\"\(key)\"}")
    guard let r = replies.first, let data = r.data(using: .utf8),
          let obj = try? JSONSerialization.jsonObject(with: data) as? [String: Any],
          let b64 = obj["text"] as? String,
          let raw = Data(base64Encoded: b64) else { return false }
    return (try? raw.write(to: URL(fileURLWithPath: dest))) != nil
}

func efsExport(_ key: String) -> String? {
    let replies = post("efs.export", "{\"path\":\"\(key)\"}")
    guard let r = replies.first, let data = r.data(using: .utf8),
          let obj = try? JSONSerialization.jsonObject(with: data) as? [String: Any],
          obj["ok"] as? Bool == true else { return nil }
    return obj["path"] as? String
}
