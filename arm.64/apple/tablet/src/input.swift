import SwiftUI

private struct PatInput: ViewModifier {
    #if os(watchOS)
    @State private var crown = 0.0
    var swipe: some Gesture {
        DragGesture(minimumDistance: 20).onEnded { g in
            let dx = g.translation.width, dy = g.translation.height
            if abs(dx) > abs(dy) { sendKey(dx > 0 ? keyRight : keyLeft) }
            else { sendKey(dy > 0 ? keyDown : keyUp) }
        }
    }
    func body(content: Content) -> some View {
        content
            .simultaneousGesture(swipe)
            .focusable()
            .digitalCrownRotation($crown)
            .onChange(of: crown) { old, new in sendKey(new > old ? keyDown : keyUp) }
    }
    #elseif os(tvOS)
    func body(content: Content) -> some View {
        content
            .focusable()
            .onMoveCommand { direction in
                switch direction {
                case .up: sendKey(keyUp)
                case .down: sendKey(keyDown)
                case .left: sendKey(keyLeft)
                case .right: sendKey(keyRight)
                @unknown default: break
                }
            }
    }
    #else
    var swipe: some Gesture {
        DragGesture(minimumDistance: 20).onEnded { g in
            let dx = g.translation.width, dy = g.translation.height
            if abs(dx) > abs(dy) { sendKey(dx > 0 ? keyRight : keyLeft) }
            else { sendKey(dy > 0 ? keyDown : keyUp) }
        }
    }
    func body(content: Content) -> some View {
        content
            .simultaneousGesture(swipe)
            .focusable()
    }
    #endif
}

extension View { func patInput() -> some View { modifier(PatInput()) } }
