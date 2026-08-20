#pragma once

#include <nlohmann/json.hpp>
#include <string>
#include <string_view>
#include <optional>

Address get_into(Options options);
extern "C" int Pump(Address address, Payload payload, Options options);

inline void send_to(const char* options,
                    const nlohmann::json& payload,
                    const nlohmann::json& options_out = nlohmann::json::object()) {
    if (!options) return;
    Address into = get_into(options);
    if (!into || !*into) return;
    std::string pay = payload.dump();
    std::string opt = options_out.empty() ? "once" : options_out.dump();
    Pump(into, pay.c_str(), opt.c_str());
}
