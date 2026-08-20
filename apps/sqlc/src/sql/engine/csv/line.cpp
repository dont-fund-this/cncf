#include "engine/csv/line.h"

std::string_view line(const Mapped& m, size_t& pos) {
    const size_t begin = pos;
    while (pos < m.size && m.data[pos] != '\n' && m.data[pos] != '\r') ++pos;
    const std::string_view out(m.data + begin, pos - begin);
    while (pos < m.size && (m.data[pos] == '\n' || m.data[pos] == '\r')) ++pos;
    return out;
}
