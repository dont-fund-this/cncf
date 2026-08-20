#include "state.h"
#include "with.h"

std::vector<Def> sql_with() {
    return {
        sql::engine::with_parse(), sql::engine::with_deploy(), sql::engine::with_query(),
        sql::engine::with_import(), sql::engine::with_import_virtual(), sql::engine::with_call(),
        sql::engine::with_run(), sql::engine::with_ingest(), sql::engine::with_stream(),
        sql::engine::with_import_real_flags(), sql::engine::with_import_virt_flags(),
        sql::meta::with_schema(), sql::meta::with_columns(), sql::meta::with_foreign_keys(),
        sql::meta::with_indexes(), sql::engine::with_explain(), sql::engine::with_bind(),
        sql::database_list::with_selected_get(), sql::database_list::with_selected_set(),
        sql::settings::with_option_set(), sql::settings::with_option_get(),
        sql::execute::history::with_add(), sql::execute::history::with_get(),
        sql::execute::history::with_selected_set(), sql::execute::history::with_selected_get(),
        sql::filter::with_add(), sql::filter::with_get(), sql::filter::with_clear(),
        sql::execute::result::with_set_add(), sql::execute::result::with_list_get(),
        sql::execute::result::with_selected_set(), sql::execute::result::with_selected_get(),
        sql::text::with_selected_set(), sql::text::with_selected_get(),
        sql::text::with_selected_execute(), sql::database_object::with_selected_set(),
        sql::database_object::with_selected_get(), sql::settings::with_log_add(),
        sql::settings::with_log_list(), sql::database_list::with_add(),
        sql::database_list::with_get(), sql::database_list::with_list(),
        sql::database_list::with_del(), sql::settings::with_save(), sql::settings::with_load(),
        sql::execute::result::with_output_record(), sql::execute::result::with_output_list(),
        sql::execute::result::with_output_get(), sql::engine::with_materialize(),
        sql::execute::result::with_page(), sql::database::with_copy(),
        sql::database::with_table_copy(), sql::engine::with_execute_into(), with_help(),
    };
}
