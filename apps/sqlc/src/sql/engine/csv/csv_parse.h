#pragma once

#include <deque>
#include <string>
#include <string_view>
#include <vector>

void csv_parse(std::string_view line, char delim, std::vector<std::string_view>& out, std::deque<std::string>& owned);
