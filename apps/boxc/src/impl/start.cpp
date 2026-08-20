#include "../type.hpp"
#include "../box/state.h"
#include <string>
#include <string_view>
#include <optional>

Address get_into(Options options);
bool has_verb(Options options, const char* verb);
std::optional<std::string_view> json_find_field(std::string_view s, std::string_view key);
extern "C" int Pump(Address address, Payload payload, Options options);

extern const Def BoxStart = {
    "box.start",
    "box.start",
    [](Address address, Payload, Options options) -> bool {
        if (!address) return false;
        std::string_view addr(address);
        return (addr == "box.start" || addr.find("/pods") != std::string_view::npos) && has_verb(options, "POST");
    },
    [](Address address, Payload payload, Options options) -> int {
        if (!BoxStart.fit(address, payload, options)) return -1;
        auto& s = box::state();
        if (s.running.load()) {
            Address into_target = get_into(options);
            if (into_target) return Pump(into_target, "{\"ok\":false,\"reason\":\"already running\"}", "once");
            return 1;
        }

        std::string bios, kernel, initrd, drive, image, shim, fs, fb;
        std::string cmdline = "console=hvc0 root=/dev/vda rw";
        std::uint64_t ram = 32;

        if (payload) {
            std::string_view p(payload);
            if (auto v = json_find_field(p, "bios")) bios = *v;
            if (auto v = json_find_field(p, "kernel")) kernel = *v;
            if (auto v = json_find_field(p, "initrd")) initrd = *v;
            if (auto v = json_find_field(p, "drive")) drive = *v;
            if (auto v = json_find_field(p, "image")) image = *v;
            if (auto v = json_find_field(p, "shim")) shim = *v;
            if (auto v = json_find_field(p, "fs")) fs = *v;
            if (auto v = json_find_field(p, "fb")) fb = *v;
            if (auto v = json_find_field(p, "cmdline")) cmdline = *v;
            if (auto v = json_find_field(p, "ram")) {
                try { ram = std::stoull(std::string(*v)); } catch (...) {}
            }
        }

        const bool ok = box::boot(bios, kernel, initrd, drive, image, shim, fs, fb, cmdline, ram);
        if (ok) {
            s.running.store(true);
            s.worker = std::thread(box::run);
        }

        std::string res = ok ? "{\"ok\":true,\"started\":true}" : "{\"ok\":false,\"started\":false}";
        Address into_target = get_into(options);
        if (into_target) {
            return Pump(into_target, res.c_str(), "once");
        }
        return ok ? 1 : 0;
    }
};
