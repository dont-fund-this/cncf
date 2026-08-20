#include "engine/csv/insert.h"
#include "engine/quote_ident.h"

std::string insert(const std::string& table, size_t ncol, size_t nrows) {
    std::string group = "(";
    for (size_t i = 0; i < ncol; ++i) group += (i ? ",?" : "?");
    group += ")";
    std::string out = "INSERT INTO " + quote_ident(table) + " VALUES ";
    for (size_t r = 0; r < nrows; ++r) out += (r ? "," : "") + group;
    return out;
}
