#include "b64_encode.h"

static const char kAlphabet[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

std::string b64_encode(const char* data, std::size_t size) {
    std::string out;
    out.reserve(((size + 2) / 3) * 4);

    std::size_t i = 0;
    const auto* p = reinterpret_cast<const unsigned char*>(data);

    while (i + 3 <= size) {
        unsigned v = (p[i] << 16) | (p[i + 1] << 8) | p[i + 2];
        out.push_back(kAlphabet[(v >> 18) & 0x3F]);
        out.push_back(kAlphabet[(v >> 12) & 0x3F]);
        out.push_back(kAlphabet[(v >>  6) & 0x3F]);
        out.push_back(kAlphabet[ v        & 0x3F]);
        i += 3;
    }

    const std::size_t rem = size - i;
    if (rem == 1) {
        unsigned v = p[i] << 16;
        out.push_back(kAlphabet[(v >> 18) & 0x3F]);
        out.push_back(kAlphabet[(v >> 12) & 0x3F]);
        out.push_back('=');
        out.push_back('=');
    } else if (rem == 2) {
        unsigned v = (p[i] << 16) | (p[i + 1] << 8);
        out.push_back(kAlphabet[(v >> 18) & 0x3F]);
        out.push_back(kAlphabet[(v >> 12) & 0x3F]);
        out.push_back(kAlphabet[(v >>  6) & 0x3F]);
        out.push_back('=');
    }
    return out;
}
