#include "engine/csv/map_close.h"

#include <sys/mman.h>

void map_close(const Mapped& m) {
    if (m.data) ::munmap(const_cast<char*>(m.data), m.size);
}
