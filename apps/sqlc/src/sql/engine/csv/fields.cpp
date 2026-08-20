#include "engine/csv/fields.h"

void fields(std::string_view line, std::vector<std::string_view>& out, char delim) {
    out.clear();
    size_t begin = 0;
    for (size_t i = 0; i <= line.size(); ++i) {
        if (i == line.size() || line[i] == delim) {
            out.push_back(line.substr(begin, i - begin));
            begin = i + 1;
        }
    }
}
