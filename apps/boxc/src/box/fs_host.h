#pragma once

#include "emu.h"

#include <string>

// Host-directory virtio-9p backend, owned by box.
//
// tinyemu's fs.c provides the 9P protocol core + virtio-9p transport (the
// CONSUMER of an FSDevice). The host-side provider is box's job — exactly as
// box owns BlockDevice (make_block) and the console CharacterDevice. Upstream's
// fs_disk.c is Bellard's Linux reference impl and is NOT in EMU_SRCS (it pulls
// <sys/statfs.h>/<sys/sysmacros.h>, Linux-only). make_fs is the Darwin/POSIX
// production impl of the same public fs.h interface: portable on the mac host
// and watchOS, in-mem (virtio rings, no network), no deps edit, no fork.
//
// Derived structurally from tinyemu fs_disk.c @ pinned _pm_/tinyemu/2019-12-21
// (MIT, © 2016 Fabrice Bellard); only the three Linux-isms are platform-shimmed.

namespace box {

FSDevice* make_fs(const std::string& root);
void      free_fs(FSDevice* fs);

}
