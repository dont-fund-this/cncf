#include "state.h"

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#include "emu.h"

namespace box {

bool setup_shim(const std::string& path, void* vm) {
    if (path.empty() || !vm) return false;
    auto& s = state();
    bool ro = false;
    int fd = ::open(path.c_str(), O_RDWR);
    if (fd < 0) { fd = ::open(path.c_str(), O_RDONLY); ro = true; }
    if (fd < 0) return false;
    struct stat st {};
    if (::fstat(fd, &st) != 0 || st.st_size <= 0) { ::close(fd); return false; }
    const std::uint64_t size =
        static_cast<std::uint64_t>(st.st_size) & ~(static_cast<std::uint64_t>(DEVRAM_PAGE_SIZE) - 1);
    if (size == 0) { ::close(fd); return false; }
    void* ptr = ::mmap(nullptr, size, ro ? PROT_READ : (PROT_READ | PROT_WRITE), MAP_SHARED, fd, 0);
    if (ptr == MAP_FAILED) { ::close(fd); return false; }
    auto* con = static_cast<VirtMachine*>(vm)->console_dev;
    auto* mem_map = *reinterpret_cast<PhysMemoryMap**>(con);
    PhysMemoryRange* pr = register_ram_entry(mem_map, SHIM_ADDR, size, ro ? DEVRAM_FLAG_ROM : 0);
    pr->phys_mem = static_cast<std::uint8_t*>(ptr);
    s.shim.ptr  = ptr;
    s.shim.pr   = pr;
    s.shim.fd   = fd;
    s.shim.size = size;
    return true;
}

}
