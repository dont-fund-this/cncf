#pragma once

#include "engine/import/flags.h"
#include <nlohmann/json.hpp>
#include <functional>
#include <string>

nlohmann::json build_import_frames(const std::string& target,
                                   const std::string& table,
                                   const std::string& file,
                                   const std::function<void(long)>& progress = {},
                                   const Flags& flags = {});
