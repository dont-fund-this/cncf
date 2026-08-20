#pragma once

#include <nlohmann/json.hpp>
#include <string>

nlohmann::json build_deploy_reply(const std::string& db);
