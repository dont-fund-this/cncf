#include "../../../../../type.hpp"
#include <nlohmann/json.hpp>
#include <string>
#include <string_view>
#include <vector>
#include <optional>
#include <cctype>

Address get_into(Options options);
bool has_verb(Options options, const char* verb);
extern "C" int Pump(Address address, Payload payload, Options options);

static int hex_val(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    return -1;
}

static std::string extract_name(Payload payload) {
    if (!payload) return "";
    auto j = nlohmann::json::parse(payload, nullptr, false);
    if (!j.is_discarded()) {
        if (j.contains("metadata") && j["metadata"].is_object() && j["metadata"].contains("name") && j["metadata"]["name"].is_string()) {
            return j["metadata"]["name"].get<std::string>();
        }
        if (j.contains("name") && j["name"].is_string()) {
            return j["name"].get<std::string>();
        }
        if (j.contains("hex") && j["hex"].is_string()) {
            std::string hex_str = j["hex"].get<std::string>();
            std::vector<uint8_t> raw;
            raw.reserve(hex_str.size() / 2);
            for (size_t i = 0; i + 1 < hex_str.size(); i += 2) {
                int h = hex_val(hex_str[i]);
                int l = hex_val(hex_str[i + 1]);
                if (h >= 0 && l >= 0) raw.push_back((h << 4) | l);
            }

            if (raw.size() > 20 && raw[0] == 'k' && raw[1] == '8' && raw[2] == 's' && raw[3] == '\0') {
                for (size_t i = 20; i + 2 < raw.size(); ++i) {
                    if (raw[i] == 0x0a) {
                        size_t len = raw[i + 1];
                        if (len > 0 && len <= 63 && i + 2 + len <= raw.size()) {
                            bool valid = true;
                            for (size_t k = 0; k < len; ++k) {
                                char c = (char)raw[i + 2 + k];
                                if (!isalnum(c) && c != '-') { valid = false; break; }
                            }
                            if (valid) return std::string((char*)&raw[i + 2], len);
                        }
                    }
                }
            }
        }
    }

    return "";
}

static std::optional<std::string_view> extract_namespace(std::string_view addr) {
    std::string_view prefix = "/api/v1/namespaces/";
    std::string_view rel_prefix = "namespaces/";

    std::string_view rest;
    if (addr.rfind(prefix, 0) == 0) {
        rest = addr.substr(prefix.size());
    } else if (addr.rfind(rel_prefix, 0) == 0) {
        rest = addr.substr(rel_prefix.size());
    } else {
        return std::nullopt;
    }

    auto slash = rest.find('/');
    if (slash == std::string_view::npos) return std::nullopt;

    std::string_view ns = rest.substr(0, slash);
    std::string_view after = rest.substr(slash);

    if (after == "/serviceaccounts" && !ns.empty()) {
        return ns;
    }
    return std::nullopt;
}

extern const Def ServiceAccountPost = {
    "/api/v1/namespaces/serviceaccounts",
    "/api/v1/namespaces/serviceaccounts",
    [](Address address, Payload, Options options) -> bool {
        if (!address) return false;
        std::string_view addr(address);
        return extract_namespace(addr).has_value() && has_verb(options, "POST");
    },
    [](Address address, Payload payload, Options options) -> int {
        if (!ServiceAccountPost.fit(address, payload, options)) return -1;
        std::string_view addr(address);
        auto ns_opt = extract_namespace(addr);
        if (!ns_opt.has_value()) return -1;
        std::string ns(*ns_opt);

        std::string sa = extract_name(payload);
        if (sa.empty()) return -1;

        nlohmann::json res = {
            {"apiVersion", "v1"},
            {"kind", "ServiceAccount"},
            {"metadata", {
                {"name", sa},
                {"namespace", ns},
                {"uid", "1"},
                {"resourceVersion", "1"}
            }}
        };

        std::string res_str = res.dump();
        Address into_target = get_into(options);
        if (into_target) {
            return Pump(into_target, res_str.c_str(), "once");
        }
        return 1;
    }
};
