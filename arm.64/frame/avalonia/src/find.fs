namespace Pat

open System
open System.IO

module Finder =
    let find (targetDir: string option) : string list =
        let dir =
            match targetDir with
            | Some d when not (String.IsNullOrEmpty d) -> d
            | _ ->
                let envDir = Environment.GetEnvironmentVariable "DIST_DIR"
                if not (String.IsNullOrEmpty envDir) then envDir
                else
                    let candidates = [
                        Path.GetFullPath(Path.Combine(AppContext.BaseDirectory, "../../../dist"))
                        Path.GetFullPath(Path.Combine(AppContext.BaseDirectory, "../../dist"))
                        "dist"
                        "../../dist"
                    ]
                    candidates
                    |> List.tryFind Directory.Exists
                    |> Option.defaultValue "dist"

        if not (Directory.Exists dir) then []
        else
            try
                Directory.GetFiles dir
                |> Array.filter (fun f -> Path.GetFileName f <> ".DS_Store")
                |> Array.map Path.GetFullPath
                |> Array.toList
            with _ -> []
