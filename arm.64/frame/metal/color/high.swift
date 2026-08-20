import SwiftUI

func render(_ raw: String) -> AttributedString {
    let frame = raw.split(separator: "\n", omittingEmptySubsequences: false).suffix(32).joined(separator: "\n")
    let s = Array(frame.unicodeScalars)
    var out = AttributedString()
    var fg = Color.white
    var bg: Color? = nil
    var text = ""
    var i = 0
    func emit() {
        if text.isEmpty { return }
        var a = AttributedString(text)
        a.foregroundColor = fg
        if let bg { a.backgroundColor = bg }
        out.append(a)
        text = ""
    }
    while i < s.count {
        if s[i] == "\u{1B}", i + 1 < s.count, s[i + 1] == "[" {
            emit(); i += 2; var code = ""
            while i < s.count, s[i] != "m", s[i] != "\u{1B}" { code.unicodeScalars.append(s[i]); i += 1 }
            if i < s.count, s[i] == "m" { (fg, bg) = sgr(code, fg, bg); i += 1 }
        } else {
            text.unicodeScalars.append(s[i]); i += 1
        }
    }
    emit()
    return out
}

func sgr(_ code: String, _ fg: Color, _ bg: Color?) -> (Color, Color?) {
    let p = code.split(separator: ";").map { Int($0) ?? -1 }
    var f = fg, b = bg
    var i = 0
    while i < p.count {
        let c = p[i]
        if c == 0 { f = .white; b = nil; i += 1 }
        else if c == 38, i + 4 < p.count, p[i + 1] == 2 { f = rgb(p[i + 2], p[i + 3], p[i + 4]); i += 5 }
        else if c == 48, i + 4 < p.count, p[i + 1] == 2 { b = rgb(p[i + 2], p[i + 3], p[i + 4]); i += 5 }
        else if let named = ansiFg(c) { f = named; i += 1 }
        else { i += 1 }
    }
    return (f, b)
}

func rgb(_ r: Int, _ g: Int, _ b: Int) -> Color {
    Color(red: Double(r) / 255, green: Double(g) / 255, blue: Double(b) / 255)
}

func ansiFg(_ c: Int) -> Color? {
    let m: [Int: Color] = [30: .black, 31: .red, 32: .green, 33: .yellow, 34: .blue, 35: .purple, 36: .cyan, 37: .white,
                           90: .gray, 91: .red, 92: .green, 93: .yellow, 94: .blue, 95: .purple, 96: .cyan, 97: .white]
    return m[c]
}
