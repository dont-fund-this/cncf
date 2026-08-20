#include "csv.h"

void csv_args(int argc, const char* const* argv, CsvOpts& out) {
    for (int i = 3; i < argc; ++i) {
        const std::string a = argv[i];
        const auto eq = a.find('=');
        if (eq == std::string::npos) continue;
        std::string val = a.substr(eq + 1);
        if (val.size() >= 2 && (val.front() == '\'' || val.front() == '"')) val = val.substr(1, val.size() - 2);
        if (a.compare(0, eq, "filename") == 0)                 out.filename = val;
        if (a.compare(0, eq, "header") == 0)                   out.header = (val == "true" || val == "1");
        if (a.compare(0, eq, "delimiter") == 0 && !val.empty()) out.delim = val[0];
        if (a.compare(0, eq, "schema") == 0)                   out.schema = val;
    }
}
