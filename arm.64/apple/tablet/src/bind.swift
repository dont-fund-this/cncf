import Foundation

func bind(_ path: String) -> Abi? {
    guard let h = dlopen(path, RTLD_NOW) else { return nil }
    guard let a = dlsym(h, "attach"), let d = dlsym(h, "detach"),
          let i = dlsym(h, "invoke"), let r = dlsym(h, "report") else {
        dlclose(h)
        return nil
    }
    return Abi(handle: h,
               attach: unsafeBitCast(a, to: AttachFn.self),
               detach: unsafeBitCast(d, to: DetachFn.self),
               invoke: unsafeBitCast(i, to: InvokeFn.self),
               report: unsafeBitCast(r, to: ReportFn.self))
}
