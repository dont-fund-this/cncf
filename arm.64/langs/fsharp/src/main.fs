namespace Pat

open System

module Program =
    [<EntryPoint>]
    let main args =
        let targetDir = if args.Length > 0 then Some args.[0] else None
        let dist = Booter.boot targetDir

        if not (List.isEmpty dist) then
            let trips = Tripper.trip ()
            for d in dist do
                for t in trips do
                    d.Pump.Invoke(t.Address, t.Payload, t.Options) |> ignore

        printfn "{\n  \"lang\": \"fsharp\",\n  \"status\": \"ready\",\n  \"engines\": %d\n}" dist.Length
        0
