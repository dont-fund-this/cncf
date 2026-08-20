import Foundation

final class Session {
    var console = ""
    var live = false
}

let session = Session()
let useFb = true

func good(_ replies: [String]) -> Bool {
    guard let reply = replies.first, let data = reply.data(using: .utf8),
          let object = try? JSONSerialization.jsonObject(with: data) as? [String: Any]
    else { return false }
    return object["ok"] as? Bool == true
}

func start() {
    print("[PAT] start() called")
    guard !session.live else {
        print("[PAT] session already live")
        return
    }
    if jam == nil {
        _ = prep()
    }
    grid.image = nil
    grid.showText = false
    print("[PAT] boot config: \(bootDef.cmdline), ram=\(bootDef.ram)MB, fb=\(bootDef.fbW)x\(bootDef.fbH)")
    guard prepareMounts() else {
        print("[PAT] prepareMounts() FAILED")
        return
    }
    let cfg = primaryCfg()
    print("[PAT] box.prep payload: \(cfg)")
    let prepOk = good(post("box.prep", cfg))
    print("[PAT] box.prep result: \(prepOk)")
    guard prepOk else { return }
    
    let startOk = good(post("box.start", "{}"))
    print("[PAT] box.start result: \(startOk)")
    guard startOk else { return }
    
    session.live = true
    print("[PAT] TinyEMU VM successfully booted and live!")
}

func step() {
    guard session.live else { return }
    let replies = post("box.poll", "{}")
    for r in replies {
        absorb(r, session: session)
    }
    _ = post("box.fb", "{}") { frame($0) }
}

func stop() {
    if session.live {
        print("[PAT] stop() called")
        _ = post("box.stop", "{}")
    }
    session.live = false
    unprep()
}
