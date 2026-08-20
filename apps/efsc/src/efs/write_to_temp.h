#pragma once

#include <cstdint>
#include <filesystem>
#include <string>

std::filesystem::path write_to_temp(const std::string& path, const char* start,
                                    uint64_t size, std::string& err);
