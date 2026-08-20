import Foundation

func main() {
    let args = CommandLine.arguments
    let targetDir = args.count > 1 ? args[1] : nil
    let dist = boot(targetDir: targetDir)

    if !dist.isEmpty {
        let trips = trip()
        for d in dist {
            for t in trips {
                t.address.withCString { cAddr in
                    t.payload.withCString { cPay in
                        t.options.withCString { cOpt in
                            _ = d.Pump(cAddr, cPay, cOpt)
                        }
                    }
                }
            }
        }
    }

    let out = """
{
  "framework": "metal",
  "status": "ready",
  "engines": \(dist.count)
}
"""
    print(out)
}

main()
