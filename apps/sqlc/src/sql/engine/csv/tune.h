#pragma once

#include "engine/import/flags.h"
#include <sqlite3.h>

void tune(sqlite3* db, const Flags& flags = {});
