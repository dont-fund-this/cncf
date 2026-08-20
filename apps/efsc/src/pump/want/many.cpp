#include "../../type.hpp"
#include <string_view>

namespace want {
    bool many(Options options) {
        if (!options) return false;
        std::string_view opt(options);
        return opt == "many";
    }
}
