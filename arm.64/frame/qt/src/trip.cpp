#include "type.h"

std::vector<Triplet> trip() {
    return {
        {"/version", "{}", "{\"once\":true}"},
        {"/storage", "{}", "{\"once\":true}"},
        {"sql.help", "{}", "{\"once\":true}"}
    };
}
