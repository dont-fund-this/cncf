#include "engine/deploy/parse_deploy.h"
#include "engine/parse/trim.h"

#include <sstream>

std::vector<DeployEntry> parse_deploy(const std::string& rc) {
    std::vector<DeployEntry> out;
    std::istringstream in(rc);
    std::string line;
    while (std::getline(in, line)) {
        if (!line.empty() && line[0] == '#') continue;
        const auto at = line.find('@');
        if (at == std::string::npos) continue;
        DeployEntry e{ trim(line.substr(0, at)), trim(line.substr(at + 1)) };
        if (e.path.empty()) continue;
        out.push_back(e);
    }
    return out;
}
