#include "state.h"

#include <cstdio>
#include <cstdlib>

namespace box {

unsigned char* read_file(const std::string& path, long* out_len) {
    *out_len = 0;
    if (path.empty()) return nullptr;
    std::FILE* f = std::fopen(path.c_str(), "rb");
    if (!f) return nullptr;
    std::fseek(f, 0, SEEK_END);
    const long n = std::ftell(f);
    std::fseek(f, 0, SEEK_SET);
    if (n <= 0) { std::fclose(f); return nullptr; }
    auto* buf = static_cast<unsigned char*>(std::malloc(static_cast<std::size_t>(n)));
    if (!buf) { std::fclose(f); return nullptr; }
    const std::size_t got = std::fread(buf, 1, static_cast<std::size_t>(n), f);
    std::fclose(f);
    if (got != static_cast<std::size_t>(n)) { std::free(buf); return nullptr; }
    *out_len = n;
    return buf;
}

}
