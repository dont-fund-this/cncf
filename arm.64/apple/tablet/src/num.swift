import Foundation

func numv(_ s: String, _ key: String) -> UInt {
    let bytes = Array(s.utf8)
    let k = Array("\"\(key)\"".utf8)
    var i = 0
    while i + k.count <= bytes.count {
        if Array(bytes[i ..< i + k.count]) == k {
            var j = i + k.count
            while j < bytes.count && (bytes[j] == 0x20 || bytes[j] == 0x3a) { j += 1 }
            var n: UInt = 0
            var any = false
            while j < bytes.count && bytes[j] >= 0x30 && bytes[j] <= 0x39 {
                n = n * 10 + UInt(bytes[j] - 0x30); j += 1; any = true
            }
            if any { return n }
        }
        i += 1
    }
    return 0
}
