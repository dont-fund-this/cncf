#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <filesystem>
#include <tree_sitter/api.h>

extern "C" {
    const TSLanguage *tree_sitter_rust(void);
    const TSLanguage *tree_sitter_cpp(void);
    const TSLanguage *tree_sitter_go(void);
}

namespace fs = std::filesystem;

std::string normalize_unit(const fs::path &root, const fs::path &file) {
    fs::path rel = fs::relative(file, root);
    std::string rel_str = rel.string();

    if (rel_str.find(".DS_Store") != std::string::npos) return "";

    std::string filename = file.filename().string();
    if (filename == "mod.rs" || rel_str == "impl/impl.cpp" || rel_str == "impl/all.go") return "";
    if (rel_str == "defs/libs/libs.hpp" || rel_str == "pump/iter/iter.hpp" || rel_str == "pump/json/json.hpp") return "";
    if (rel_str == "type.hpp" || rel_str == "type.rs" || rel_str == "type/type.go") return "type";

    size_t last_dot = rel_str.find_last_of(".");
    if (last_dot != std::string::npos) {
        std::string ext = rel_str.substr(last_dot);
        if (ext == ".rs" || ext == ".cpp" || ext == ".go") {
            rel_str = rel_str.substr(0, last_dot);
        }
    }
    return rel_str;
}

int main(int argc, char **argv) {
    std::string fogr_dir = (argc > 1) ? argv[1] : "../fogr/src";
    std::string fogc_dir = (argc > 2) ? argv[2] : "../fogc/src";
    std::string fogg_dir = (argc > 3) ? argv[3] : "../fogg/src";

    TSParser *parser_rust = ts_parser_new();
    ts_parser_set_language(parser_rust, tree_sitter_rust());

    TSParser *parser_cpp = ts_parser_new();
    ts_parser_set_language(parser_cpp, tree_sitter_cpp());

    TSParser *parser_go = ts_parser_new();
    ts_parser_set_language(parser_go, tree_sitter_go());

    bool failed = false;

    // 1. Extract Golden Functional Units from Rust (fogr)
    std::set<std::string> golden_units;
    for (const auto &entry : fs::recursive_directory_iterator(fogr_dir)) {
        if (!entry.is_regular_file()) continue;
        std::string unit = normalize_unit(fogr_dir, entry.path());
        if (!unit.empty()) golden_units.insert(unit);
    }

    // 2. Extract C++ Units (fogc)
    std::set<std::string> fogc_units;
    for (const auto &entry : fs::recursive_directory_iterator(fogc_dir)) {
        if (!entry.is_regular_file()) continue;
        std::string unit = normalize_unit(fogc_dir, entry.path());
        if (!unit.empty()) fogc_units.insert(unit);
    }

    // 3. Extract Go Units (fogg)
    std::set<std::string> fogg_units;
    for (const auto &entry : fs::recursive_directory_iterator(fogg_dir)) {
        if (!entry.is_regular_file()) continue;
        std::string unit = normalize_unit(fogg_dir, entry.path());
        if (!unit.empty()) fogg_units.insert(unit);
    }

    // 4. Enforce Strict Golden Path Unit Parity
    for (const auto &unit : golden_units) {
        if (fogc_units.find(unit) == fogc_units.end()) {
            std::cerr << "LINT ERROR: Missing unit in C++ (fogc): " << unit << std::endl;
            failed = true;
        }
        if (fogg_units.find(unit) == fogg_units.end()) {
            std::cerr << "LINT ERROR: Missing unit in Go (fogg): " << unit << std::endl;
            failed = true;
        }
    }

    // Check for unapproved drift units in C++ or Go
    for (const auto &unit : fogc_units) {
        if (golden_units.find(unit) == golden_units.end()) {
            std::cerr << "LINT ERROR: Untracked drift unit in C++ (fogc): " << unit << std::endl;
            failed = true;
        }
    }
    for (const auto &unit : fogg_units) {
        if (golden_units.find(unit) == golden_units.end()) {
            std::cerr << "LINT ERROR: Untracked drift unit in Go (fogg): " << unit << std::endl;
            failed = true;
        }
    }

    // 5. Tree-Sitter AST Parse Validation across all source files
    std::vector<std::pair<std::string, std::pair<std::string, TSParser*>>> targets = {
        {fogr_dir, {"rust", parser_rust}},
        {fogc_dir, {"cpp", parser_cpp}},
        {fogg_dir, {"go", parser_go}}
    };

    for (const auto &t : targets) {
        std::string dir = t.first;
        std::string lang = t.second.first;
        TSParser *parser = t.second.second;

        if (!fs::exists(dir)) continue;

        for (const auto &entry : fs::recursive_directory_iterator(dir)) {
            if (!entry.is_regular_file()) continue;
            fs::path p = entry.path();
            std::string ext = p.extension().string();

            if ((lang == "rust" && ext == ".rs") ||
                (lang == "cpp" && (ext == ".cpp" || ext == ".hpp")) ||
                (lang == "go" && ext == ".go")) {

                std::ifstream f(p);
                std::stringstream buf;
                buf << f.rdbuf();
                std::string code = buf.str();

                TSTree *tree = ts_parser_parse_string(parser, nullptr, code.c_str(), code.length());
                if (!tree) {
                    std::cerr << "LINT PARSE FAILURE: " << p.string() << std::endl;
                    failed = true;
                } else {
                    TSNode root_node = ts_tree_root_node(tree);
                    if (ts_node_has_error(root_node)) {
                        std::cerr << "LINT SYNTAX ERROR: " << p.string() << std::endl;
                        failed = true;
                    }
                    ts_tree_delete(tree);
                }
            }
        }
    }

    ts_parser_delete(parser_rust);
    ts_parser_delete(parser_cpp);
    ts_parser_delete(parser_go);

    if (failed) {
        std::cerr << "\nLINT FAILED: Parity drift or syntax errors detected across variants.\n";
        return 1;
    }

    return 0;
}
