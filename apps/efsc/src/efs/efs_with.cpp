#include "state.h"

Def with_load();
Def with_size();
Def with_read();
Def with_peek();
Def with_list();
Def with_export();
Def with_sniff();
Def with_help();

std::vector<Def> efs_with() {
    return {
        with_load(),
        with_size(),
        with_read(),
        with_peek(),
        with_list(),
        with_export(),
        with_sniff(),
        with_help(),
    };
}
