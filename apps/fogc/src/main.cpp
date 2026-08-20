#include "type.hpp"
#include <unistd.h>
#include <cstring>
#include <string_view>
#include <vector>

int more(Def def);
int less(Def def);
extern "C" int Pump(Address address, Payload payload, Options options);
std::vector<Trip> trip();

int main() {
    Def some_def = {
        "some-id",
        "thing1",
        [](Address address, Payload, Options) -> bool {
            if (!address) return false;
            std::string_view addr(address);
            return addr == "some-id" || addr == "some-def";
        },
        [](Address, Payload payload, Options) -> int {
            if (payload) {
                write(1, payload, strlen(payload));
                write(1, "\n", 1);
            }
            return 0;
        }
    };

    Def some_other_def = {
        "some-other-id",
        "thing2",
        [](Address address, Payload, Options) -> bool {
            if (!address) return false;
            return std::string_view(address) == "some-other-id";
        },
        [](Address, Payload payload, Options) -> int {
            if (payload) {
                write(1, payload, strlen(payload));
                write(1, "\n", 1);
            }
            return 0;
        }
    };

    more(some_def);
    more(some_other_def);

    for (const auto& t : trip()) {
        Pump(t.address, t.payload, t.options);
    }

    less(some_def);
    less(some_other_def);

    return 0;
}
