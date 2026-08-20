#include "../type.hpp"

namespace want {
    bool none(Options options);
    bool once(Options options);
    bool many(Options options);
}

int none(Address address, Payload payload, Options options);
int once(Address address, Payload payload, Options options);
int many(Address address, Payload payload, Options options);

int pump_impl(Address address, Payload payload, Options options) {
    if (want::none(options)) return none(address, payload, options);
    if (want::once(options)) return once(address, payload, options);
    if (want::many(options)) return many(address, payload, options);
    return -1;
}
