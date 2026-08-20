#pragma once

#include "../type.hpp"
#include <vector>

Def with_help();

namespace sql::engine {
Def with_parse();
Def with_deploy();
Def with_query();
Def with_import();
Def with_import_virtual();
Def with_call();
Def with_run();
Def with_ingest();
Def with_stream();
Def with_import_real_flags();
Def with_import_virt_flags();
Def with_explain();
Def with_bind();
Def with_materialize();
Def with_execute_into();
}

namespace sql::meta {
Def with_columns();
Def with_schema();
Def with_indexes();
Def with_foreign_keys();
}

namespace sql::text {
Def with_selected_get();
Def with_selected_set();
Def with_selected_execute();
}

namespace sql::database_object {
Def with_selected_get();
Def with_selected_set();
}

namespace sql::settings {
Def with_load();
Def with_save();
Def with_option_get();
Def with_option_set();
Def with_log_add();
Def with_log_list();
}

namespace sql::execute::history {
Def with_add();
Def with_get();
Def with_selected_get();
Def with_selected_set();
}

namespace sql::execute::result {
Def with_set_add();
Def with_list_get();
Def with_page();
Def with_selected_get();
Def with_selected_set();
Def with_output_get();
Def with_output_list();
Def with_output_record();
}

namespace sql::database {
Def with_copy();
Def with_table_copy();
}

namespace sql::database_list {
Def with_add();
Def with_get();
Def with_list();
Def with_del();
Def with_selected_get();
Def with_selected_set();
}

namespace sql::filter {
Def with_add();
Def with_get();
Def with_clear();
}

std::vector<Def> sql_with();
