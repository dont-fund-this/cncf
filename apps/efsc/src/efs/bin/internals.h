#pragma once
#include <cstdint>
#include <string>
#include <vector>

struct ParsedEntry { std::string path; uint64_t offset; uint64_t size; };

namespace bin {

bool open_mmap(const std::string& path, void*& map, size_t& bytes, std::string& err);
bool parse_header(const uint8_t* base, size_t bytes, uint32_t& count, std::string& err);
bool parse_entries(const uint8_t* base, size_t bytes, uint32_t count,
                   std::vector<ParsedEntry>& out, std::string& err);

}
