#pragma once

#include "pool.h"
#include <cstddef>
#include <string>

struct BytesView {
    const char* data   = nullptr;
    std::size_t size   = 0;
    bool        ok     = false;
    std::string error;
};

BytesView bytes_of(const std::string& path);
