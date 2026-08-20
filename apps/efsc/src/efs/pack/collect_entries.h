#pragma once

#include "file_entry.h"

#include <algorithm>
#include <filesystem>
#include <string>
#include <vector>

namespace pack {

inline std::vector<FileEntry> collect_entries(const std::string& source_dir) {
    namespace fs = std::filesystem;
    std::vector<FileEntry> entries;
    for (const auto& entry : fs::recursive_directory_iterator(source_dir)) {
        if (!entry.is_regular_file()) continue;
        if (entry.path().filename() == ".DS_Store") continue;
        FileEntry e;
        e.full_path = entry.path().string();
        e.rel_path  = fs::relative(entry.path(), source_dir).string();
        e.size      = fs::file_size(entry.path());
        entries.push_back(e);
    }
    std::sort(entries.begin(), entries.end(),
              [](const FileEntry& a, const FileEntry& b) {
                  return a.rel_path < b.rel_path;
              });
    return entries;
}

}
