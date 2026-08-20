local ffi = require("ffi")

ffi.cdef[[
typedef const char* Address;
typedef const char* Payload;
typedef const char* Options;

typedef struct Def Def;
typedef int (*MoreFn)(Def* def);
typedef int (*PumpFn)(Address address, Payload payload, Options options);
typedef int (*LessFn)(Def* def);
]]

return {
    Triplet = function(address, payload, options)
        return { address = address, payload = payload, options = options }
    end
}
