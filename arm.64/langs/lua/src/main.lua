local boot_mod = require("boot")
local trip_mod = require("trip")

local target_dir = arg[1]
local dist = boot_mod.boot(target_dir)

if #dist > 0 then
    local trips = trip_mod()
    for _, d in ipairs(dist) do
        for _, t in ipairs(trips) do
            d.Pump(t.address, t.payload, t.options)
        end
    end
end

print(string.format("{\n  \"lang\": \"lua\",\n  \"status\": \"ready\",\n  \"engines\": %d\n}", #dist))
