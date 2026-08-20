#include "../type.hpp"
#include "../box/state.h"
#include <string>
#include <string_view>

Address get_into(Options options);
bool has_verb(Options options, const char* verb);
extern "C" int Pump(Address address, Payload payload, Options options);

extern const Def BoxStop = {
    "box.stop",
    "box.stop",
    [](Address address, Payload, Options options) -> bool {
        if (!address) return false;
        std::string_view addr(address);
        return (addr == "box.stop" || (addr.find("/pods") != std::string_view::npos && has_verb(options, "DELETE")));
    },
    [](Address address, Payload payload, Options options) -> int {
        if (!BoxStop.fit(address, payload, options)) return -1;
        auto& s = box::state();
        if (s.running.load()) {
            s.running.store(false);
            if (s.worker.joinable()) {
                s.worker.join();
            }
            box::teardown();
        }

        Address into_target = get_into(options);
        if (into_target) {
            return Pump(into_target, "{\"ok\":true,\"stopped\":true}", "once");
        }
        return 1;
    }
};
