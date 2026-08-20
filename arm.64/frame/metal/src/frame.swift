import CoreGraphics
import Foundation

public func frame(fbData: UnsafePointer<UInt16>?, width: Int32, height: Int32) -> CGImage? {
    guard let src = fbData, width > 0, height > 0 else { return nil }
    let n = Int(width) * Int(height)
    var rgba = [UInt8](repeating: 0, count: n * 4)
    for i in 0..<n {
        let p = src[i]
        rgba[i * 4 + 0] = UInt8((p >> 11) & 0x1F) << 3 // R5 → R8
        rgba[i * 4 + 1] = UInt8((p >> 5) & 0x3F) << 2  // G6 → G8
        rgba[i * 4 + 2] = UInt8(p & 0x1F) << 3         // B5 → B8
        rgba[i * 4 + 3] = 255
    }
    guard let provider = CGDataProvider(data: Data(rgba) as CFData) else { return nil }
    return CGImage(
        width: Int(width), height: Int(height), bitsPerComponent: 8, bitsPerPixel: 32,
        bytesPerRow: Int(width) * 4, space: CGColorSpaceCreateDeviceRGB(),
        bitmapInfo: CGBitmapInfo(rawValue: CGImageAlphaInfo.premultipliedLast.rawValue
            | CGBitmapInfo.byteOrder32Big.rawValue),
        provider: provider, decode: nil, shouldInterpolate: false, intent: .defaultIntent
    )
}
