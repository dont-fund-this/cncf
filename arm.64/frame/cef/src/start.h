#pragma once

#include "include/cef_scheme.h"

void declare_efs_scheme(CefRawPtr<CefSchemeRegistrar> registrar);
void start();
