#include "state.h"

#include <chrono>
#include <thread>

#include "emu.h"

namespace box {

void run() {
    auto& s = state();
    auto* vm = static_cast<VirtMachine*>(s.machine.vm);
    if (!vm) return;
    while (s.running.load()) {
        try {
            pump_input();
            poll_doorbell();
            virt_machine_interp(vm, 100000);
            const int delay = virt_machine_get_sleep_duration(vm, 10);
            std::this_thread::sleep_for(std::chrono::milliseconds(delay > 0 ? delay : 0));
        } catch (...) {
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }
    }
}

}
