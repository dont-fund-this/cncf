#include "type.hpp"
#include "box/state.h"
#include <iostream>
#include <vector>

extern "C" {
    int More(Def def);
    int Less(Def def);
    int Pump(Address address, Payload payload, Options options);
    const std::uint16_t* box_fb(int* w, int* h);
}

size_t impl_count();
Defs impl_all();

int main(int argc, char** argv) {
    Defs defs = impl_all();
    size_t count = impl_count();
    for (size_t i = 0; i < count; ++i) {
        More(defs[i]);
    }

    if (argc > 1 && std::string(argv[1]) == "start") {
        std::string payload = (argc > 2) ? argv[2] : "{}";
        int res = Pump("box.start", payload.c_str(), "into:stdout,verb:POST");
        return res >= 0 ? 0 : 1;
    }

    std::cout << "{\n  \"app\": \"boxc\",\n  \"status\": \"ready\",\n  \"defs\": " << count << "\n}\n";
    return 0;
}
