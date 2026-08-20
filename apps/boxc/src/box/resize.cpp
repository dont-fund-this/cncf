#include "state.h"

#include "emu.h"

namespace box {

void resize(int width, int height) {
    auto& s = state();
    auto* con = static_cast<VIRTIODevice*>(s.machine.console_dev);
    if (con) virtio_console_resize_event(con, width, height);
}

}
