import SwiftUI

struct Show: View {
    @ObservedObject var grid: Grid
    @State private var input = ""
    var body: some View {
        ZStack {
            Color.black.ignoresSafeArea()
            if let img = grid.image, !grid.showText {
                Image(decorative: img, scale: 1.0)
                    .resizable()
                    .interpolation(.none)
                    .aspectRatio(contentMode: .fit)
                    .contentShape(Rectangle())
                    .onTapGesture { tapToggle() }
                    .onLongPressGesture(minimumDuration: 0.5) { sendKey(keyEsc) }
            } else {
                VStack(spacing: 2) {
                    Text(grid.text)
                        .font(.system(size: 11, design: .monospaced))
                        .minimumScaleFactor(0.05)
                        .lineLimit(50)
                        .foregroundColor(Color(red: 0, green: 229.0 / 255.0, blue: 1))
                        .multilineTextAlignment(.leading)
                        .frame(maxWidth: .infinity, maxHeight: .infinity, alignment: .topLeading)
                        .contentShape(Rectangle())
                        .onTapGesture { tapToggle() }
                        .onLongPressGesture(minimumDuration: 0.5) { sendKey(keyEsc) }
                    TextField("cmd", text: $input)
                        .font(.system(size: 13, design: .monospaced))
                        .foregroundColor(.white)
                        .autocorrectionDisabled()
                        .onSubmit { sendInput(input); input = "" }
                }
                .padding(8)
            }
        }
        .frame(maxWidth: .infinity, maxHeight: .infinity)
        .ignoresSafeArea()
        .patInput()
    }
}
