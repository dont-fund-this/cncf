import Foundation

typealias FitFn = (String, String, String) -> Bool
typealias FunFn = (String, String, String) -> Void

struct Def {
    let sid: String
    let tag: String
    let fit: FitFn
    let fun: FunFn
}

typealias InvokeFn = @convention(c) (UnsafePointer<CChar>?, UnsafePointer<CChar>?, UnsafePointer<CChar>?) -> CInt
typealias AttachFn = @convention(c) (InvokeFn?) -> Bool
typealias DetachFn = @convention(c) () -> Bool
typealias ReportFn = @convention(c) () -> PatInfo

struct Abi {
    let handle: UnsafeMutableRawPointer
    let attach: AttachFn
    let detach: DetachFn
    let invoke: InvokeFn
    let report: ReportFn
}

var jam: Abi?
