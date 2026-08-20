#include "type.hpp"
#include <iostream>

extern "C" {
    int More(Def def);
    int Less(Def def);
    int Pump(Address address, Payload payload, Options options);
}

size_t impl_count();
Defs impl_all();

int main(int argc, char** argv) {
    Defs defs = impl_all();
    size_t count = impl_count();
    for (size_t i = 0; i < count; ++i) {
        More(defs[i]);
    }

    std::cout << "{\n  \"app\": \"efsc\",\n  \"status\": \"ready\",\n  \"defs\": " << count << "\n}\n";
    return 0;
}
