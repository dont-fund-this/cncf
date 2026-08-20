#include "state.h"

#include <sys/mman.h>
#include <unistd.h>

#include "emu.h"

namespace box {

void teardown_shim() {
    auto& s = state();
    if (s.shim.pr) {
        static_cast<PhysMemoryRange*>(s.shim.pr)->phys_mem = nullptr;
        s.shim.pr = nullptr;
    }
    if (s.shim.ptr) { ::munmap(s.shim.ptr, s.shim.size); s.shim.ptr = nullptr; }
    if (s.shim.fd >= 0) { ::close(s.shim.fd); s.shim.fd = -1; }
    s.shim.size = 0;
    if (s.doorbell.pr) {
        static_cast<PhysMemoryRange*>(s.doorbell.pr)->phys_mem = nullptr;
        s.doorbell.pr = nullptr;
    }
    if (s.doorbell.ptr) { ::munmap(s.doorbell.ptr, DEVRAM_PAGE_SIZE); s.doorbell.ptr = nullptr; }
}

}
