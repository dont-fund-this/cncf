import Foundation

func strict(_ options: String) -> String? {
    guard let data = options.data(using: .utf8),
          let object = try? JSONSerialization.jsonObject(with: data) as? [String: Any],
          let value = object["strict"] as? String,
          value == "none" || value == "once" || value == "many" else { return nil }
    return value
}

let invoke: InvokeFn = { cAddress, cPayload, cOptions in
    guard let cAddress, let cPayload, let cOptions else { return 0 }
    let address = String(cString: cAddress)
    let payload = String(cString: cPayload)
    let options = String(cString: cOptions)
    guard let mode = strict(options) else { return 0 }
    let fits = appDefs.filter { $0.fit(address, payload, options) }
    if mode == "none" { return CInt(fits.count) }
    if mode == "once" {
        guard let def = fits.first else { return 0 }
        def.fun(address, payload, options)
        return 1
    }
    for def in fits { def.fun(address, payload, options) }
    return CInt(fits.count)
}
