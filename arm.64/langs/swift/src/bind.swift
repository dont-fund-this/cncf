import Foundation

func bind(_ binaryPath: String) -> Cabi? {
    let filename = (binaryPath as NSString).lastPathComponent
    let skips = ["c", "cpp", "rust", "go", "swift", "haskell", "zig", "v", "slint_sample"]
    if skips.contains(filename) { return nil }

    guard let handle = dlopen(binaryPath, RTLD_LAZY | RTLD_LOCAL) else { return nil }

    let moreSym = dlsym(handle, "More")
    let pumpSym = dlsym(handle, "Pump")
    let lessSym = dlsym(handle, "Less")

    guard let pumpPtr = pumpSym else {
        dlclose(handle)
        return nil
    }

    let moreFn = moreSym.map { unsafeBitCast($0, to: MoreFn.self) }
    let pumpFn = unsafeBitCast(pumpPtr, to: PumpFn.self)
    let lessFn = lessSym.map { unsafeBitCast($0, to: LessFn.self) }

    return Cabi(name: filename, path: binaryPath, handle: handle, more: moreFn, pump: pumpFn, less: lessFn)
}
