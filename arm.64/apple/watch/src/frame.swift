import Foundation
import CoreGraphics

func frame(_ payload: String) {
    let ptr = numv(payload, "ptr")
    let w = Int(numv(payload, "w"))
    let h = Int(numv(payload, "h"))
    guard ptr != 0, w > 0, h > 0, let src = UnsafePointer<UInt16>(bitPattern: ptr) else { return }
    var hasPixels = false
    for i in 0 ..< (w * h) {
        if src[i] != 0 {
            hasPixels = true
            break
        }
    }
    if !hasPixels { return }
    var rgba = [UInt8](repeating: 0, count: w * h * 4)
    for i in 0 ..< (w * h) {
        let p = src[i]
        rgba[i * 4]     = UInt8((Int((p >> 11) & 0x1f) * 255) / 31)
        rgba[i * 4 + 1] = UInt8((Int((p >> 5) & 0x3f) * 255) / 63)
        rgba[i * 4 + 2] = UInt8((Int(p & 0x1f) * 255) / 31)
        rgba[i * 4 + 3] = 255
    }
    guard let provider = CGDataProvider(data: Data(rgba) as CFData) else { return }
    guard let img = CGImage(width: w, height: h, bitsPerComponent: 8, bitsPerPixel: 32, bytesPerRow: w * 4,
                            space: CGColorSpaceCreateDeviceRGB(),
                            bitmapInfo: CGBitmapInfo(rawValue: CGImageAlphaInfo.premultipliedLast.rawValue),
                            provider: provider, decode: nil, shouldInterpolate: false, intent: .defaultIntent) else { return }
    DispatchQueue.main.async { grid.image = img }
}
