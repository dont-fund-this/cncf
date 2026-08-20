import SwiftUI

public struct TermView: View {
    public var screenText: String
    public var onSend: (String) -> Void
    @State private var line: String = ""
    @State private var typing: Bool = false

    public init(screenText: String, onSend: @escaping (String) -> Void) {
        self.screenText = screenText
        self.onSend = onSend
    }

    public var body: some View {
        show(image: nil, text: screenText)
            .contentShape(Rectangle())
            .onTapGesture { line = ""; typing = true }
            .sheet(isPresented: $typing) {
                VStack(spacing: 8) {
                    TextField("›", text: $line)
                        .font(.system(size: 16, design: .monospaced))
                    Button("send") {
                        if !line.isEmpty { onSend(line + "\n") }
                        line = ""
                        typing = false
                    }
                    .font(.system(size: 14, design: .monospaced))
                }
                .padding()
            }
    }
}
