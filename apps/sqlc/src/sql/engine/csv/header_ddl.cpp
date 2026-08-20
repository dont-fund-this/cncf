#include "engine/csv/header_ddl.h"
#include "engine/quote_ident.h"

std::string header_ddl(const std::vector<std::string_view>& head, bool header) {
    std::string ddl = "CREATE TABLE x(";
    for (size_t i = 0; i < head.size(); ++i) {
        if (i) ddl += ',';
        ddl += (header && !head[i].empty()) ? quote_ident(head[i]) : ("c" + std::to_string(i));
    }
    ddl += ')';
    return ddl;
}
