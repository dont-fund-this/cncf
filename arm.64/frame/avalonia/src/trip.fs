namespace Pat

module Tripper =
    let trip () : Triplet list = [
        { Address = "/version"; Payload = "{}"; Options = "{\"once\":true}" }
        { Address = "/storage"; Payload = "{}"; Options = "{\"once\":true}" }
        { Address = "sql.help"; Payload = "{}"; Options = "{\"once\":true}" }
    ]
