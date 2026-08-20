#include "type.h"
#include <iostream>

extern std::vector<Cabi*> boot(const std::string& target_dir);
extern std::vector<Triplet> trip();

int main(int argc, char* argv[]) {
    std::string target_dir = (argc > 1) ? argv[1] : "";
    std::vector<Cabi*> dist = boot(target_dir);

    if (!dist.empty()) {
        std::vector<Triplet> trips = trip();
        for (auto* d : dist) {
            for (const auto& t : trips) {
                d->Pump(t.address.c_str(), t.payload.c_str(), t.options.c_str());
            }
        }
    }

    std::cout << "{\n"
              << "  \"framework\": \"qt\",\n"
              << "  \"status\": \"ready\",\n"
              << "  \"engines\": " << dist.size() << "\n"
              << "}" << std::endl;

    return 0;
}
