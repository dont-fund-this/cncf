#include "type.hpp"
#include <vector>

std::vector<Trip> trip() {
    return {
        { "/version", "{}", "into:some-id" },
        { "/storage", "{}", "into:some-other-id" },
    };
}
