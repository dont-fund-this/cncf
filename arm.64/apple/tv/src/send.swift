import Foundation

func rawSend(_ s: String) {
    let esc = s
        .replacingOccurrences(of: "\\", with: "\\\\")
        .replacingOccurrences(of: "\"", with: "\\\"")
    _ = post("box.send", "{\"data\":\"\(esc)\"}")
}

func sendInput(_ cmd: String) {
    rawSend(cmd + "\n")
}
