FROM debian:bookworm
RUN apt-get update && apt-get install -y --no-install-recommends \
      gcc make libc6-dev coreutils \
 && rm -rf /var/lib/apt/lists/*
COPY temusrc/ /t/
WORKDIR /t
RUN DEFS='-DCONFIG_VERSION="boot-check" -D_GNU_SOURCE -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE -DCONFIG_RISCV_MAX_XLEN=64 -DMAX_XLEN=64' \
 && for n in temu virtio pci fs fs_disk fs_utils cutils iomem simplefb json machine riscv_machine softfp; do \
      cc -O2 -w $DEFS -I. -c $n.c -o $n.o; done \
 && cc -O2 -w $DEFS -DMAX_XLEN=64 -I. -c riscv_cpu.c -o riscv_cpu64.o \
 && cc -O2 -w $DEFS -DMAX_XLEN=32 -I. -c riscv_cpu.c -o riscv_cpu32.o \
 && cc -o /temu temu.o virtio.o pci.o fs.o fs_disk.o fs_utils.o cutils.o iomem.o \
       simplefb.o json.o machine.o riscv_machine.o softfp.o riscv_cpu64.o riscv_cpu32.o -lpthread -lm
ENTRYPOINT ["/bin/sh", "-c"]
