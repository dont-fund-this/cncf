#include "../type.hpp"

extern const Def VersionGet;
extern const Def NamespacePost;
extern const Def ServiceAccountPost;

static const Def ALL_DEFS[] = {
    VersionGet,
    NamespacePost,
    ServiceAccountPost,
};

size_t impl_count() {
    return 3;
}

Defs impl_all() {
    return ALL_DEFS;
}
