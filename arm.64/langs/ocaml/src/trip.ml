let trip () = [
  { Type.address = "/version"; payload = "{}"; options = "{\"once\":true}" };
  { Type.address = "/storage"; payload = "{}"; options = "{\"once\":true}" };
  { Type.address = "sql.help"; payload = "{}"; options = "{\"once\":true}" }
]
