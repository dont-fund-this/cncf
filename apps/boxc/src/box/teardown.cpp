#include "state.h"

#include "emu.h"
#include "block.h"

namespace box {

void teardown() {
    auto& s = state();
    teardown_shim();
    if (s.machine.vm) {
        virt_machine_end(static_cast<VirtMachine*>(s.machine.vm));
        s.machine.vm = nullptr;
    }
    s.machine.console_dev = nullptr;
    if (s.machine.drive) {
        free_block(static_cast<BlockDevice*>(s.machine.drive));
        s.machine.drive = nullptr;
    }
    if (s.machine.image) {
        free_block(static_cast<BlockDevice*>(s.machine.image));
        s.machine.image = nullptr;
    }
    if (s.machine.cs) {
        delete static_cast<CharacterDevice*>(s.machine.cs);
        s.machine.cs = nullptr;
    }
}

}
