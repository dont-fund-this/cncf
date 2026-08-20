#include "state.h"

#include <sys/mman.h>

#include "emu.h"

namespace box {

bool setup_doorbell(void* vm) {
    if (!vm) return false;
    auto& s = state();
    void* ptr = ::mmap(nullptr, DEVRAM_PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_ANON | MAP_SHARED, -1, 0);
    if (ptr == MAP_FAILED) return false;
    auto* con = static_cast<VirtMachine*>(vm)->console_dev;
    auto* mem_map = *reinterpret_cast<PhysMemoryMap**>(con);
    PhysMemoryRange* pr = register_ram_entry(mem_map, DOORBELL_ADDR, DEVRAM_PAGE_SIZE, 0);
    pr->phys_mem = static_cast<std::uint8_t*>(ptr);
    s.doorbell.ptr = ptr;
    s.doorbell.pr  = pr;
    return true;
}

}
