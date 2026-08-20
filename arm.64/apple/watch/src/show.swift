import SwiftUI

struct Show: View {
    @ObservedObject var grid: Grid
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
                VStack(spacing: 1) {
                    Text(grid.text)
                        .foregroundColor(Color(red: 0, green: 229.0 / 255.0, blue: 1))
                        .multilineTextAlignment(.leading)
                        .frame(maxWidth: .infinity, maxHeight: .infinity, alignment: .topLeading)
                        .contentShape(Rectangle())
                        .onTapGesture { tapToggle() }
                        .onLongPressGesture(minimumDuration: 0.5) { sendKey(keyEsc) }
                }
                .padding(2)
            }
        }
        .frame(maxWidth: .infinity, maxHeight: .infinity)
        .ignoresSafeArea()
        .patInput()
    }
}
