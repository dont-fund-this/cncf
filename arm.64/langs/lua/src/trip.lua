local type_mod = require("type")

return function()
    return {
        type_mod.Triplet("/version", "{}", "{\"once\":true}"),
        type_mod.Triplet("/storage", "{}", "{\"once\":true}"),
        type_mod.Triplet("sql.help", "{}", "{\"once\":true}")
    }
end
