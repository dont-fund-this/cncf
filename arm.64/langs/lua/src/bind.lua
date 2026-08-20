local ffi = require("ffi")

local M = {}

function M.bind(binary_path)
    local filename = binary_path:match("([^/]+)$")
    local skips = { c=true, cpp=true, rust=true, go=true, swift=true, haskell=true, zig=true, v=true, slint_sample=true }
    if skips[filename] then return nil end

    local ok, lib = pcall(ffi.load, binary_path)
    if not ok then return nil end

    local pump_ok, pump = pcall(function() return lib.Pump end)
    if not pump_ok or not pump then return nil end

    return {
        name = filename,
        path = binary_path,
        lib = lib,
        Pump = pump
    }
end

return M
