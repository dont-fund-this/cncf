import Foundation

let targetDir = CommandLine.arguments.count > 1 ? CommandLine.arguments[1] : nil
let dist = boot(targetDir)
if !dist.isEmpty {
    let trips = trip()
    for d in dist {
        for t in trips {
            t.address.withCString { addr in
                t.payload.withCString { pay in
                    t.options.withCString { opt in
                        _ = d.pump(addr, pay, opt)
                    }
                }
            }
        }
    }
}

print("""
{
  "lang": "swift",
  "status": "ready",
  "engines": \(dist.count)
}
""")
