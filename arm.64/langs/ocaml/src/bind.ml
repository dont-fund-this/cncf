external caml_dlopen : string -> nativeint option = "caml_dlopen"
external caml_dlsym_pump : nativeint option -> nativeint option = "caml_dlsym_pump"
external caml_call_pump : nativeint option -> string -> string -> string -> int = "caml_call_pump"

let bind binary_path =
  let filename = Filename.basename binary_path in
  let skips = ["c"; "cpp"; "rust"; "go"; "swift"; "haskell"; "zig"; "v"; "slint_sample"] in
  if List.mem filename skips then None
  else
    match caml_dlopen binary_path with
    | None -> None
    | Some h -> (
        match caml_dlsym_pump (Some h) with
        | None -> None
        | Some p -> Some { Type.name = filename; path = binary_path; pump_fn = p }
      )
