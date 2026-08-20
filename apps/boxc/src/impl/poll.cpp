#include "../type.hpp"
#include "../box/state.h"
#include <string>
#include <string_view>
#include <vector>

Address get_into(Options options);
bool has_verb(Options options, const char* verb);
extern "C" int Pump(Address address, Payload payload, Options options);

extern const Def BoxPoll = {
    "box.poll",
    "box.poll",
    [](Address address, Payload, Options options) -> bool {
        if (!address) return false;
        std::string_view addr(address);
        return (addr == "box.poll" || addr.find("/status") != std::string_view::npos || addr.find("/logs") != std::string_view::npos) && has_verb(options, "GET");
    },
    [](Address address, Payload payload, Options options) -> int {
        if (!BoxPoll.fit(address, payload, options)) return -1;
        auto& s = box::state();
        std::string out;
        std::vector<std::uint32_t> sigs;
        {
            std::lock_guard<std::mutex> lk(s.chan.io);
            out.swap(s.chan.output);
            sigs.swap(s.doorbell.signals);
        }

        std::size_t nz = 0;
        std::uint16_t sample = 0;
        if (s.fb.ptr) {
            const auto* px = static_cast<const std::uint16_t*>(s.fb.ptr);
            for (std::size_t i = 0, n = s.fb.size / 2; i < n; ++i) {
                if (px[i]) {
                    ++nz;
                    if (!sample) sample = px[i];
                }
            }
        }

        std::string res = "{\"ok\":true,\"running\":" + std::string(s.running.load() ? "true" : "false") +
                          ",\"nonzero\":" + std::to_string(nz) +
                          ",\"sample\":" + std::to_string(sample) +
                          ",\"w\":" + std::to_string(s.fb.w) +
                          ",\"h\":" + std::to_string(s.fb.h) + "}";

        Address into_target = get_into(options);
        if (into_target) {
            return Pump(into_target, res.c_str(), "once");
        }
        return 1;
    }
};
