#include "engine/deploy/read_text.h"
#include <fstream>
#include <sstream>

std::string read_text(const std::string& path) {
    std::ifstream f(path);
    if (!f.is_open()) return "";
    std::stringstream ss;
    ss << f.rdbuf();
    return ss.str();
}
