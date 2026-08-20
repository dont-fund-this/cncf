#include "state.h"

#include <sys/mman.h>

#include "emu.h"

namespace box {

// RAM-backed framebuffer at FB_ADDR (RGB565, 2 bytes/px). Mirrors setup_shim but anonymous
// (no host file): rata's FB backend mmaps /dev/mem here and draws pixels; box reads s.fb.ptr
// in-process and ships it to the watch. The guest needs only /dev/mem — no kernel FB driver.
bool setup_fb(int w, int h, void* vm) {
    if (w <= 0 || h <= 0 || !vm) return false;
    auto& s = state();
    const std::uint64_t raw = static_cast<std::uint64_t>(w) * static_cast<std::uint64_t>(h) * 2;
    const std::uint64_t size =
        (raw + DEVRAM_PAGE_SIZE - 1) & ~(static_cast<std::uint64_t>(DEVRAM_PAGE_SIZE) - 1);
    void* ptr = ::mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANON, -1, 0);
    if (ptr == MAP_FAILED) return false;
    auto* con = static_cast<VirtMachine*>(vm)->console_dev;
    auto* mem_map = *reinterpret_cast<PhysMemoryMap**>(con);
    PhysMemoryRange* pr = register_ram_entry(mem_map, FB_ADDR, size, 0);
    pr->phys_mem = static_cast<std::uint8_t*>(ptr);
    s.fb.ptr  = ptr;
    s.fb.pr   = pr;
    s.fb.size = size;
    s.fb.w    = w;
    s.fb.h    = h;
    return true;
}

}

// In-process pixel getter for the host (the watch app, which loads box as a framework): returns
// the live RGB565 framebuffer pointer + dims, or nullptr if no FB. The guest writes it
// concurrently; tearing is harmless (it's a framebuffer stream), so no lock.
extern "C" const std::uint16_t* box_fb(int* w, int* h) {
    auto& s = box::state();
    if (w) *w = s.fb.w;
    if (h) *h = s.fb.h;
    return static_cast<const std::uint16_t*>(s.fb.ptr);
}
