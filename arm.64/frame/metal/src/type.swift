import Foundation

public typealias Address = UnsafePointer<CChar>
public typealias Payload = UnsafePointer<CChar>
public typealias Options = UnsafePointer<CChar>

public typealias FitFn = @convention(c) (Address?, Payload?, Options?) -> CBool
public typealias FunFn = @convention(c) (Address?, Payload?, Options?) -> CInt

public struct Def {
    public var sid: UnsafePointer<CChar>?
    public var tag: UnsafePointer<CChar>?
    public var fit: FitFn?
    public var fun: FunFn?

    public init(sid: UnsafePointer<CChar>?, tag: UnsafePointer<CChar>?, fit: FitFn?, fun: FunFn?) {
        self.sid = sid
        self.tag = tag
        self.fit = fit
        self.fun = fun
    }
}

public typealias MoreFn = @convention(c) (UnsafeRawPointer?) -> CInt
public typealias LessFn = @convention(c) (UnsafeRawPointer?) -> CInt
public typealias PumpFn = @convention(c) (Address?, Payload?, Options?) -> CInt

public struct Cabi {
    public let name: String
    public let path: String
    public let lib: UnsafeMutableRawPointer
    public let More: MoreFn
    public let Less: LessFn
    public let Pump: PumpFn
}

public struct Triplet {
    public let address: String
    public let payload: String
    public let options: String

    public init(address: String, payload: String = "{}", options: String = "{\"once\":true}") {
        self.address = address
        self.payload = payload
        self.options = options
    }
}
