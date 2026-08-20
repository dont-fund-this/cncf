#pragma once

#include "engine/deploy/deploy_entry.h"

#include <string>
#include <vector>

std::vector<DeployEntry> parse_deploy(const std::string& rc);
