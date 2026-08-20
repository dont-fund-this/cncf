#include "../type.hpp"
#include "../box/state.h"
#include <string>
#include <string_view>
#include <optional>

Address get_into(Options options);
bool has_verb(Options options, const char* verb);
std::optional<std::string_view> json_find_field(std::string_view s, std::string_view key);
extern "C" int Pump(Address address, Payload payload, Options options);

extern const Def BoxSend = {
    "box.send",
    "box.send",
    [](Address address, Payload, Options options) -> bool {
        if (!address) return false;
        std::string_view addr(address);
        return (addr == "box.send" || addr.find("/exec") != std::string_view::npos) && has_verb(options, "POST");
    },
    [](Address address, Payload payload, Options options) -> int {
        if (!BoxSend.fit(address, payload, options)) return -1;
        auto& s = box::state();
        std::string data;
        if (payload) {
            std::string_view p(payload);
            if (auto v = json_find_field(p, "data")) {
                data = *v;
            } else {
                data = p;
            }
        }
        {
            std::lock_guard<std::mutex> lk(s.chan.io);
            s.chan.input += data;
        }

        std::string res = "{\"ok\":true,\"queued\":" + std::to_string(data.size()) + "}";
        Address into_target = get_into(options);
        if (into_target) {
            return Pump(into_target, res.c_str(), "once");
        }
        return 1;
    }
};
