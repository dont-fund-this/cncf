#pragma once

#include <string_view>
#include <vector>

void fields(std::string_view line, std::vector<std::string_view>& out, char delim = ',');
