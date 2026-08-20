import Foundation

public func bind(binaryPath: String) -> Cabi? {
    let filename = (binaryPath as NSString).lastPathComponent
    let skip = ["c", "cpp", "rust", "go", "swift", "haskell", "zig", "v"]
    if skip.contains(filename) {
        return nil
    }

    guard let lib = dlopen(binaryPath, RTLD_LAZY) else {
        return nil
    }

    guard let moreSym = dlsym(lib, "More"),
          let pumpSym = dlsym(lib, "Pump"),
          let lessSym = dlsym(lib, "Less") else {
        dlclose(lib)
        return nil
    }

    let more = unsafeBitCast(moreSym, to: MoreFn.self)
    let pump = unsafeBitCast(pumpSym, to: PumpFn.self)
    let less = unsafeBitCast(lessSym, to: LessFn.self)

    return Cabi(
        name: filename,
        path: binaryPath,
        lib: lib,
        More: more,
        Less: less,
        Pump: pump
    )
}
