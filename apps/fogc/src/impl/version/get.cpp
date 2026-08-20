#include "../../type.hpp"
#include <string_view>

Address get_into(Options options);
bool has_verb(Options options, const char* verb);
extern "C" int Pump(Address address, Payload payload, Options options);

static const char version_json[] = "{\n  \"major\": \"1\",\n  \"minor\": \"36\",\n  \"gitVersion\": \"v1.36.0\",\n  \"gitCommit\": \"fogr-dev\",\n  \"gitTreeState\": \"clean\",\n  \"buildDate\": \"2026-08-16T00:00:00Z\",\n  \"goVersion\": \"go1.23.0\",\n  \"compiler\": \"gc\",\n  \"platform\": \"darwin/arm64\"\n}";

extern const Def VersionGet = {
    "version",
    "tag,any",
    [](Address address, Payload, Options options) -> bool {
        if (!address) return false;
        std::string_view addr(address);
        return (addr == "/version" || addr == "version") && has_verb(options, "GET");
    },
    [](Address address, Payload payload, Options options) -> int {
        if (!VersionGet.fit(address, payload, options)) return -1;
        Address into_target = get_into(options);
        if (into_target) {
            return Pump(into_target, version_json, "once");
        }
        return 1;
    }
};
