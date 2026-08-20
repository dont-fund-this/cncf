#include "state.h"

namespace box {

void poll_doorbell() {
    auto& s = state();
    if (!s.doorbell.ptr) return;
    auto* d = static_cast<volatile std::uint32_t*>(s.doorbell.ptr);
    const std::uint32_t v = d[0];
    if (v == 0) return;
    d[0] = 0;
    std::lock_guard<std::mutex> lk(s.chan.io);
    s.doorbell.signals.push_back(v);
}

}
