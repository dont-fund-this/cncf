#include "../../type.hpp"
#include <cstddef>

extern "C" int Pump(Address address, Payload payload, Options options);

int pump_iter(Address address, const char** items, size_t count, Options options) {
    if (!address || !items) return -1;
    for (size_t i = 0; i < count; ++i) {
        if (items[i]) {
            Pump(address, items[i], options);
        }
    }
    return (int)count;
}
