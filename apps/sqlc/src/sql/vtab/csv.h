#pragma once

#include "engine/csv/mapped.h"
#include <sqlite3.h>
#include <cstddef>
#include <deque>
#include <string>
#include <string_view>
#include <vector>

struct CsvOpts {
    std::string filename;
    bool        header = false;
    char        delim  = ',';
    std::string schema;
};

struct CsvVtab {
    sqlite3_vtab      base;
    Mapped            src;
    size_t            body  = 0;
    int               ncol  = 0;
    char              delim = ',';
    std::vector<char> aff;
};

struct CsvCursor {
    sqlite3_vtab_cursor          base;
    CsvVtab*                     vt = nullptr;
    size_t                       pos = 0;
    std::vector<std::string_view> row;
    std::deque<std::string>      owned;
    sqlite3_int64                rowid = 0;
    bool                         eof = false;
};

int  register_csv(sqlite3* db);
void csv_args(int argc, const char* const* argv, CsvOpts& out);
void csv_advance(CsvCursor* cur);

int csv_connect(sqlite3* db, void* aux, int argc, const char* const* argv, sqlite3_vtab** ppVtab, char** pzErr);
int csv_disconnect(sqlite3_vtab* vt);
int csv_bestindex(sqlite3_vtab* vt, sqlite3_index_info* info);
int csv_open(sqlite3_vtab* vt, sqlite3_vtab_cursor** ppCur);
int csv_close(sqlite3_vtab_cursor* cur);
int csv_filter(sqlite3_vtab_cursor* cur, int idxNum, const char* idxStr, int argc, sqlite3_value** argv);
int csv_next(sqlite3_vtab_cursor* cur);
int csv_eof(sqlite3_vtab_cursor* cur);
int csv_column(sqlite3_vtab_cursor* cur, sqlite3_context* ctx, int i);
int csv_rowid(sqlite3_vtab_cursor* cur, sqlite3_int64* pRowid);
