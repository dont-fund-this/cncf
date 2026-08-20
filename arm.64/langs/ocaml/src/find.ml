let find target_dir =
  let dir =
    match target_dir with
    | Some d -> d
    | None -> (
        match Sys.getenv_opt "DIST_DIR" with
        | Some d -> d
        | None ->
            let candidates = ["../../dist"; "../../../dist"; "dist"] in
            let rec check = function
              | [] -> "dist"
              | c :: cs -> if Sys.file_exists c && Sys.is_directory c then c else check cs
            in check candidates
      )
  in
  if not (Sys.file_exists dir && Sys.is_directory dir) then []
  else
    Sys.readdir dir
    |> Array.to_list
    |> List.filter (fun f -> f <> ".DS_Store")
    |> List.map (Filename.concat dir)
    |> List.filter (fun f -> not (Sys.is_directory f))
