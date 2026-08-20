#include "../type.hpp"

Defs with();
size_t with_count();

int many(Address address, Payload payload, Options options) {
    int count = 0;
    size_t total = with_count();
    Defs defs = with();
    for (size_t i = 0; i < total; i++) {
        if (defs[i].fit(address, payload, options)) {
            defs[i].fun(address, payload, options);
            count++;
        }
    }
    return count;
}
