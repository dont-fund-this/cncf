import Foundation

func prep() -> Bool {
    for path in find() {
        guard let abi = bind(path) else { continue }
        let info = abi.report()
        let sig = info.sig.map { String(cString: $0) }
        let tag = info.tag.map { String(cString: $0) }
        guard sig == "jam", tag == "jam", abi.attach(invoke) else {
            dlclose(abi.handle)
            continue
        }
        jam = abi
        if good(post("jam.init", "{}")) { return true }
        unprep()
    }
    return false
}

func unprep() {
    guard let abi = jam else { return }
    _ = abi.detach()
    dlclose(abi.handle)
    jam = nil
}
