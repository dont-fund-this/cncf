#pragma once
#include "../../type.hpp"
#include <dlfcn.h>

void* open_lib(const char* path);
Def* bind_symbol(void* handle, const char* name);
int close_lib(void* handle);
