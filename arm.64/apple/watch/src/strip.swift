import Foundation

func strip(_ s: String) -> String {
    var r = ""
    var i = s.startIndex
    while i < s.endIndex {
        let c = s[i]
        if c == "\u{1b}" {
            var j = s.index(after: i)
            if j < s.endIndex && s[j] == "[" {
                j = s.index(after: j)
                while j < s.endIndex {
                    let d = s[j]
                    j = s.index(after: j)
                    if d >= "@" && d <= "~" { break }
                }
            }
            i = j
        } else if c == "\r" {
            i = s.index(after: i)
        } else {
            r.append(c)
            i = s.index(after: i)
        }
    }
    return r
}
