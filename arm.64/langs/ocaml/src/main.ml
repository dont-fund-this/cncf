let () =
  let target_dir = if Array.length Sys.argv > 1 then Some Sys.argv.(1) else None in
  let dist = Boot.boot target_dir in
  if dist <> [] then (
    let trips = Trip.trip () in
    List.iter (fun d ->
      List.iter (fun t ->
        ignore (Bind.caml_call_pump (Some d.Type.pump_fn) t.Type.address t.Type.payload t.Type.options)
      ) trips
    ) dist
  );
  Printf.printf "{\n  \"lang\": \"ocaml\",\n  \"status\": \"ready\",\n  \"engines\": %d\n}\n" (List.length dist)
