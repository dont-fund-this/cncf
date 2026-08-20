#pragma once

#include "engine/import/flags.h"
#include <nlohmann/json.hpp>
#include <string>

nlohmann::json build_virtual_frames(const std::string& target,
                                    const std::string& table,
                                    const std::string& file,
                                    const Flags& flags = {});
