#include "list_under.h"
#include "state.h"

#include <algorithm>
#include <vector>

namespace {
struct Entry {
    std::string name;
    bool        is_dir;
    uint64_t    size;
};
}

nlohmann::json list_under(const std::string& in_path) {
    std::string path = in_path;
    while (!path.empty() && path.back() == '/') path.pop_back();
    const std::string prefix = path.empty() ? "" : path + "/";

    std::vector<Entry> entries;
    auto& s = efs::state();
    PoolReadLock lock(s.pool_mutex);
    for (const auto& kv : s.pool.index) {
        const std::string& key = kv.first;
        if (!prefix.empty() && key.find(prefix) != 0) continue;

        const std::string rest = prefix.empty() ? key : key.substr(prefix.size());
        if (rest.empty()) continue;

        const auto slash = rest.find('/');
        if (slash == std::string::npos)
            entries.push_back({rest, false, kv.second.size});
        else
            entries.push_back({rest.substr(0, slash), true, 0});
    }
    std::sort(entries.begin(), entries.end(), [](const Entry& a, const Entry& b) {
        if (a.is_dir != b.is_dir) return a.is_dir > b.is_dir;
        return a.name < b.name;
    });
    entries.erase(std::unique(entries.begin(), entries.end(), [](const Entry& a, const Entry& b) {
        return a.is_dir == b.is_dir && a.name == b.name;
    }), entries.end());

    nlohmann::json arr = nlohmann::json::array();
    for (const auto& e : entries) {
        nlohmann::json obj = {{"name", e.name}, {"type", e.is_dir ? "dir" : "file"}};
        if (!e.is_dir) obj["size"] = e.size;
        arr.push_back(obj);
    }
    return arr;
}
