import Foundation

let keyUp = "\u{1b}[A"
let keyDown = "\u{1b}[B"
let keyLeft = "\u{1b}[D"
let keyRight = "\u{1b}[C"
let keyEsc = "\u{1b}"

func sendKey(_ seq: String) {
    rawSend(seq)
}
