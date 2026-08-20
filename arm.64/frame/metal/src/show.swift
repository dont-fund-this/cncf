import SwiftUI

public func show(image: CGImage?, text: String = "") -> some View {
    Group {
        if let img = image {
            Image(decorative: img, scale: 1, orientation: .up)
                .resizable()
                .interpolation(.none)
                .aspectRatio(contentMode: .fit)
                .frame(maxWidth: .infinity, maxHeight: .infinity)
        } else {
            Text(text)
                .font(.system(size: 6, design: .monospaced))
                .minimumScaleFactor(0.05)
                .lineLimit(32)
                .frame(maxWidth: .infinity, maxHeight: .infinity)
        }
    }
    .ignoresSafeArea()
}
