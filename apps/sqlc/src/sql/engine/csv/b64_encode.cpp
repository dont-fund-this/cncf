#include "engine/csv/b64_encode.h"

std::string b64_encode(const unsigned char* data, std::size_t n) {
    static const char* const T = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    std::string out;
    for (std::size_t i = 0; i < n; i += 3) {
        const unsigned v = (static_cast<unsigned>(data[i]) << 16)
                         | (i + 1 < n ? static_cast<unsigned>(data[i + 1]) << 8 : 0)
                         | (i + 2 < n ? static_cast<unsigned>(data[i + 2]) : 0);
        out += T[(v >> 18) & 63];
        out += T[(v >> 12) & 63];
        out += (i + 1 < n) ? T[(v >> 6) & 63] : '=';
        out += (i + 2 < n) ? T[v & 63] : '=';
    }
    return out;
}
