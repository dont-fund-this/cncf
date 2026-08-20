#include "state.h"

#include <algorithm>

#include "emu.h"

namespace box {

void pump_input() {
    auto& s = state();
    auto* con = static_cast<VIRTIODevice*>(s.machine.console_dev);
    if (!con) return;
    std::lock_guard<std::mutex> lk(s.chan.io);
    if (s.chan.input.empty()) return;
    const int room = virtio_console_get_write_len(con);
    if (room <= 0) return;
    const int take = std::min(static_cast<int>(s.chan.input.size()), room);
    virtio_console_write_data(con, reinterpret_cast<const std::uint8_t*>(s.chan.input.data()), take);
    s.chan.input.erase(0, static_cast<std::size_t>(take));
}

}
