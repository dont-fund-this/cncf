#include "pack/collect_entries.h"
#include "pack/assign_offsets.h"
#include "pack/write_header.h"
#include "pack/write_blobs.h"

#include <filesystem>
#include <fstream>
#include <ios>
#include <string>

namespace efs {
bool pack_refs(const std::string& source_dir, const std::string& output_file) {
    namespace fs = std::filesystem;
    if (!fs::exists(source_dir)) return false;

    auto entries = pack::collect_entries(source_dir);
    pack::assign_offsets(entries);

    std::ofstream out(output_file, std::ios::binary);
    if (!out) return false;
    pack::write_header(out, entries);
    pack::write_blobs(out, entries);
    out.close();
    return true;
}
}
