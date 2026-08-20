#include "state.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "emu.h"
#include "block.h"
#include "fs_host.h"

namespace box {

bool boot(const std::string& bios, const std::string& kernel,
          const std::string& initrd, const std::string& drive,
          const std::string& image, const std::string& shim,
          const std::string& fs, const std::string& fb,
          const std::string& cmdline, std::uint64_t ram) {
    auto& s = state();

    long klen = 0;
    unsigned char* kbuf = read_file(kernel, &klen);
    if (!kbuf) return false;
    long blen = 0;
    unsigned char* bbuf = read_file(bios, &blen);
    long ilen = 0;
    unsigned char* ibuf = read_file(initrd, &ilen);

    auto* cd = new CharacterDevice{};
    cd->opaque     = &s;
    cd->write_data = on_write;
    cd->read_data  = on_read;
    s.machine.cs = cd;

    VirtMachineParams p;
    virt_machine_set_defaults(&p);
    p.vmc          = &riscv_machine_class;
    p.machine_name = strdup("riscv64");
    p.vmc->virt_machine_set_defaults(&p);
    p.ram_size = ram << 20;
    // Wall-clock RTC: the guest timer advances on real time, so an idle WFI guest (e.g. a
    // fully-idle PID 1 in nanosleep) wakes on schedule. Without this the clock is cycle-based
    // and freezes during WFI — the guest would never wake. Enables low-cpu timed work.
    p.rtc_real_time = 1;
    // simplefb (a8r8g8b8 @ 0x41000000) is wired here when the FB kernel + pipe land:
    //   p.display_device = strdup("simplefb"); p.width = 396; p.height = 484;
    // requires a guest kernel with CONFIG_FB_SIMPLE (+ FRAMEBUFFER_CONSOLE) — the bellard one lacks it.
    p.console  = cd;
    int fbw = 0, fbh = 0;
    if (!fb.empty()) std::sscanf(fb.c_str(), "%dx%d", &fbw, &fbh);
    std::string cmd = cmdline + shim_suffix(shim);
    if (fbw > 0 && fbh > 0) {
        // tell rata where to draw: PAT_FB=<phys addr>,<w>x<h> (RGB565 via /dev/mem)
        char fbarg[64];
        std::snprintf(fbarg, sizeof fbarg, " PAT_FB=0x%llx,%dx%d",
                      static_cast<unsigned long long>(FB_ADDR), fbw, fbh);
        cmd += fbarg;
    }
    vm_add_cmdline(&p, cmd.c_str());
    if (bbuf) { p.files[VM_FILE_BIOS].buf = bbuf; p.files[VM_FILE_BIOS].len = static_cast<int>(blen); }
    p.files[VM_FILE_KERNEL].buf = kbuf; p.files[VM_FILE_KERNEL].len = static_cast<int>(klen);
    if (ibuf) { p.files[VM_FILE_INITRD].buf = ibuf; p.files[VM_FILE_INITRD].len = static_cast<int>(ilen); }

    BlockDevice* drv = drive.empty() ? nullptr : make_block(drive);
    if (drv) { p.tab_drive[0].block_dev = drv; p.drive_count = 1; }
    BlockDevice* img = image.empty() ? nullptr : make_block(image);
    if (img) { p.tab_drive[p.drive_count].block_dev = img; p.drive_count++; }

    // virtio-9p: export a host directory the guest mounts as `mount -t 9p host …`
    // (box-owned host FSDevice — see fs_host.cpp). In-mem virtio rings, no network.
    FSDevice* fsdev = fs.empty() ? nullptr : make_fs(fs);
    if (fsdev) { p.tab_fs[0].fs_dev = fsdev; p.tab_fs[0].tag = strdup("host"); p.fs_count = 1; }

    VirtMachine* vm = virt_machine_init(&p);
    virt_machine_free_config(&p);
    if (!vm) { if (drv) free_block(drv); if (img) free_block(img); if (fsdev) free_fs(fsdev); delete cd; s.machine.cs = nullptr; return false; }
    s.machine.vm          = vm;
    s.machine.console_dev = vm->console_dev;
    s.machine.drive       = drv;
    s.machine.image       = img;
    if (!shim.empty()) { setup_shim(shim, vm); setup_doorbell(vm); }
    if (fbw > 0 && fbh > 0) setup_fb(fbw, fbh, vm);
    return true;
}

}
