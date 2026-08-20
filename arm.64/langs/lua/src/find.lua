local ffi = require("ffi")

local M = {}

function M.find(target_dir)
    local dir = target_dir or os.getenv("DIST_DIR")
    if not dir then
        local candidates = {
            "dist",
            "../../dist",
            "../../../dist"
        }
        for _, c in ipairs(candidates) do
            local f = io.open(c, "r")
            if f then
                f:close()
                dir = c
                break
            end
        end
        if not dir then dir = "dist" end
    end

    local files = {}
    local p = io.popen("ls -1 " .. dir .. " 2>/dev/null")
    if p then
        for file in p:lines() do
            if file ~= ".DS_Store" then
                table.insert(files, dir .. "/" .. file)
            end
        end
        p:close()
    end
    return files
end

return M
