#pragma once

#include "engine/csv/mapped.h"
#include <cstddef>
#include <string_view>

std::string_view line(const Mapped& m, size_t& pos);
