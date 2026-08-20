namespace Pat

open System
open System.IO
open System.Linq

module Finder =
    let find (targetDir: string option) : string list =
        let dir =
            match targetDir with
            | Some d when not (String.IsNullOrEmpty d) -> d
            | _ ->
                let env = Environment.GetEnvironmentVariable "DIST_DIR"
                if not (String.IsNullOrEmpty env) then env
                else
                    let candidates = [
                        Path.GetFullPath(Path.Combine(AppContext.BaseDirectory, "../../../dist"))
                        Path.GetFullPath(Path.Combine(AppContext.BaseDirectory, "../../dist"))
                        Path.GetFullPath(Path.Combine(Directory.GetCurrentDirectory(), "dist"))
                        "dist"
                    ]
                    candidates |> List.tryFind Directory.Exists |> Option.defaultValue "dist"

        if not (Directory.Exists dir) then []
        else
            Directory.GetFiles dir
            |> Array.filter (fun f -> Path.GetFileName f <> ".DS_Store")
            |> Array.toList
