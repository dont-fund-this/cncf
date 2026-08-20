namespace Pat

open System

module Booter =
    let boot (targetDir: string option) : Cabi list =
        let envLib = Environment.GetEnvironmentVariable "PAT_LIB"
        if not (String.IsNullOrEmpty envLib) then
            match Binder.bind envLib with
            | Some bound -> [ bound ]
            | None -> []
        else
            Finder.find targetDir
            |> List.choose Binder.bind
