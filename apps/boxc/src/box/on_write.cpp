#include "state.h"

namespace box {

void on_write(void* opaque, const std::uint8_t* buf, int len) {
    if (!opaque || !buf || len <= 0) return;
    auto* s = static_cast<State*>(opaque);
    std::lock_guard<std::mutex> lk(s->chan.io);
    s->chan.output.append(reinterpret_cast<const char*>(buf), static_cast<std::size_t>(len));
}

}
