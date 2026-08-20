#include "bin/internals.h"

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

namespace bin {

bool open_mmap(const std::string& path, void*& map, size_t& bytes, std::string& err) {
    int fd = open(path.c_str(), O_RDONLY);
    if (fd == -1) { err = "Failed to open file: " + path; return false; }

    struct stat sb{};
    if (fstat(fd, &sb) == -1) {
        close(fd);
        err = "Failed to stat file: " + path;
        return false;
    }

    void* m = mmap(nullptr, sb.st_size, PROT_READ, MAP_SHARED, fd, 0);
    close(fd);
    if (m == MAP_FAILED) { err = "mmap failed for file: " + path; return false; }

    map   = m;
    bytes = static_cast<size_t>(sb.st_size);
    return true;
}

}
