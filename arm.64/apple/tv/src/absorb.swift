import Foundation

func absorb(_ payload: String, session: Session) {
    guard let data = payload.data(using: .utf8),
          let obj = try? JSONSerialization.jsonObject(with: data) as? [String: Any],
          let out = obj["out"] as? String, !out.isEmpty else { return }
    print("[GUEST LOG] \(out)", terminator: "")
    session.console += out
    if let r = session.console.range(of: "\u{1b}[2J", options: .backwards) {
        session.console = String(session.console[r.upperBound...])
    }
    if session.console.count > 16000 { session.console = String(session.console.suffix(8000)) }
    let shown = strip(session.console)
    let tail = shown.split(separator: "\n", omittingEmptySubsequences: false).suffix(50).joined(separator: "\n")
    let devId = (Bundle.main.object(forInfoDictionaryKey: "PATDev") as? String) ?? "apple-tv-alpi"
    let header = "[Booting: \(devId) | RAM: \(bootDef.ram)MB | FB: \(bootDef.fbW)x\(bootDef.fbH)]\n\n"
    DispatchQueue.main.async { grid.text = header + tail }
}
