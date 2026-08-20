let boot target_dir =
  match Sys.getenv_opt "PAT_LIB" with
  | Some lib when lib <> "" -> (
      match Bind.bind lib with
      | Some c -> [c]
      | None -> []
    )
  | _ ->
      Find.find target_dir
      |> List.filter_map Bind.bind
