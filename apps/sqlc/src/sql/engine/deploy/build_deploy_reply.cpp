#include "engine/deploy/build_deploy_reply.h"
#include "engine/deploy/read_text.h"
#include "engine/deploy/parse_deploy.h"
#include "engine/parse/statements.h"

nlohmann::json build_deploy_reply(const std::string& db) {
    using nlohmann::json;
    const std::string base = "data/" + db;

    const std::string rc = read_text(base + "/deploy.rc");
    if (rc.empty()) return {{"ok", false}, {"error", "deploy.rc not found for db: " + db}};

    json steps = json::array();
    for (const auto& e : parse_deploy(rc)) {
        const std::string sql = read_text(base + "/" + e.path);
        steps.push_back({
            {"desc",  e.desc},
            {"path",  e.path},
            {"count", static_cast<int>(statements(sql).size())},
        });
    }
    return {{"ok", true}, {"db", db}, {"steps", steps}};
}
