import SwiftUI
import CoreGraphics

final class Grid: ObservableObject {
    @Published var image: CGImage? = nil
    @Published var text: String = "booting…"
    @Published var showText = false
}

let grid = Grid()
