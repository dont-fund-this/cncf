#pragma once

#include <nlohmann/json.hpp>
#include <map>
#include <string>
#include <vector>

namespace sql {

struct State {
    std::string                        selected;
    std::map<std::string, std::string> options;
    std::vector<std::string>           history;
    int                                selected_execute = -1;
    nlohmann::json                     filters = nlohmann::json::array();
    nlohmann::json                     results = nlohmann::json::array();
    int                                selected_result = -1;
    std::string                        selected_object;
    std::string                        selected_object_type;
    std::string                        selected_text;
    nlohmann::json                     quiver = nlohmann::json::array();
};

State& state();

}
