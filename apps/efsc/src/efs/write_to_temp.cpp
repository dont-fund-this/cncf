#include "write_to_temp.h"

#include <fstream>
#include <system_error>

std::filesystem::path write_to_temp(const std::string& path, const char* start,
                                    uint64_t size, std::string& err) {
    std::error_code ec;
    auto tmp_dir = std::filesystem::temp_directory_path(ec) / "pat";
    if (ec) { err = "tmpdir failed: " + ec.message(); return {}; }

    auto out_path = tmp_dir / std::filesystem::path(path);
    std::filesystem::create_directories(out_path.parent_path(), ec);
    if (ec) { err = "mkdir failed: " + ec.message(); return {}; }

    std::ofstream out(out_path, std::ios::binary | std::ios::trunc);
    if (!out) { err = "open failed: " + out_path.string(); return {}; }
    out.write(start, static_cast<std::streamsize>(size));
    out.close();
    if (!out) { err = "write failed: " + out_path.string(); return {}; }
    return out_path;
}
