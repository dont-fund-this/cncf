import Foundation

public typealias Address = UnsafePointer<CChar>?
public typealias Payload = UnsafePointer<CChar>?
public typealias Options = UnsafePointer<CChar>?
public typealias Sid = UnsafePointer<CChar>?
public typealias Tag = UnsafePointer<CChar>?

public typealias Fit = @convention(c) (Address, Payload, Options) -> CBool
public typealias Fun = @convention(c) (Address, Payload, Options) -> CInt

public struct Def {
    public var sid: Sid
    public var tag: Tag
    public var fit: Fit?
    public var fun: Fun?
}

public typealias MoreFn = @convention(c) (UnsafeRawPointer?) -> CInt
public typealias PumpFn = @convention(c) (Address, Payload, Options) -> CInt
public typealias LessFn = @convention(c) (UnsafeRawPointer?) -> CInt

public struct Cabi {
    public var name: String
    public var path: String
    public var handle: UnsafeMutableRawPointer
    public var more: MoreFn?
    public var pump: PumpFn
    public var less: LessFn?
}

public struct Triplet {
    public var address: String
    public var payload: String
    public var options: String
}
