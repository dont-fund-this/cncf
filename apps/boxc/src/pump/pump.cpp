#include "../type.hpp"

int pump_impl(Address address, Payload payload, Options options);

extern "C" int Pump(Address address, Payload payload, Options options) {
    return pump_impl(address, payload, options);
}
