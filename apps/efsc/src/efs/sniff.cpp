#include "sniff.h"

#include <cstring>

std::string sniff(const unsigned char* d, std::size_t n) {
    if (n == 0) return "empty";
    if (n >= 16 && std::memcmp(d, "SQLite format 3", 15) == 0) return "sqlite";
    if (n >= 4  && d[0] == 'P' && d[1] == 'K' && (d[2] == 3 || d[2] == 5) && (d[3] == 4 || d[3] == 6)) return "zip";
    if (n >= 2  && d[0] == 0x1f && d[1] == 0x8b) return "gzip";
    if (n >= 5  && std::memcmp(d, "%PDF-", 5) == 0) return "pdf";
    if (n >= 8  && d[0] == 0x89 && d[1] == 'P' && d[2] == 'N' && d[3] == 'G') return "png";
    if (n >= 3  && d[0] == 0xFF && d[1] == 0xD8 && d[2] == 0xFF) return "jpeg";
    if (n >= 6  && std::memcmp(d, "GIF8", 4) == 0) return "gif";

    const std::size_t sample = n < 512 ? n : 512;
    std::size_t bad = 0;
    for (std::size_t i = 0; i < sample; ++i) {
        const unsigned char c = d[i];
        if (!(c == 9 || c == 10 || c == 13 || (c >= 32 && c <= 126))) ++bad;
    }
    return bad * 10 < sample * 3 ? "text" : "bin";
}
