#include "../../type.hpp"
#include <string_view>

namespace want {
    bool once(Options options) {
        if (!options) return false;
        std::string_view opt(options);
        return opt == "once" || opt.rfind("into:", 0) == 0 ||
               opt.find("\"once\":true") != std::string_view::npos ||
               opt.find("\"once\": true") != std::string_view::npos;
    }
}
