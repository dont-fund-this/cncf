#include "type.h"
#include <iostream>

std::vector<Cabi*> boot(const std::string& target_dir);
std::vector<Triplet> trip();

int main(int argc, char* argv[]) {
    std::string target_dir = (argc > 1) ? argv[1] : "";
    auto dist = boot(target_dir);

    if (!dist.empty()) {
        auto trips = trip();
        for (auto* d : dist) {
            for (const auto& t : trips) {
                d->Pump(t.address.c_str(), t.payload.c_str(), t.options.c_str());
            }
        }
    }

    std::cout << "{\n"
              << "  \"lang\": \"cpp\",\n"
              << "  \"status\": \"ready\",\n"
              << "  \"engines\": " << dist.size() << "\n"
              << "}\n";
    return 0;
}
