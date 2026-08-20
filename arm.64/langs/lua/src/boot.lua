local find_mod = require("find")
local bind_mod = require("bind")

local M = {}

function M.boot(target_dir)
    local engines = {}
    local env_lib = os.getenv("PAT_LIB")
    if env_lib and env_lib ~= "" then
        local c = bind_mod.bind(env_lib)
        if c then
            table.insert(engines, c)
            return engines
        end
    end

    local files = find_mod.find(target_dir)
    for _, file in ipairs(files) do
        local c = bind_mod.bind(file)
        if c then
            table.insert(engines, c)
        end
    end
    return engines
end

return M
