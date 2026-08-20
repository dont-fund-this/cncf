#include "engine/csv/b64_decode.h"
#include "engine/csv/b64_val.h"

std::string b64_decode(const std::string& s) {
    std::string out;
    int buf = 0, bits = 0;
    for (char c : s) {
        const int v = b64_val(c);
        if (v < 0) continue;
        buf = (buf << 6) | v;
        bits += 6;
        if (bits < 8) continue;
        bits -= 8;
        out += static_cast<char>((buf >> bits) & 0xFF);
    }
    return out;
}
