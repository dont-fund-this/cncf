#include "../type.hpp"

Defs with();
size_t with_count();

int none(Address address, Payload payload, Options options) {
    size_t count = with_count();
    Defs defs = with();
    for (size_t i = 0; i < count; i++) {
        if (defs[i].fit(address, payload, options)) {
            return defs[i].fun(address, payload, options);
        }
    }
    return -1;
}
