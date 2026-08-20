include $(CURDIR)/env.mk

ALPINE := $(DEPS)/alpine
ALPINE_VERSION := $(shell cat "$(ALPINE)/.version")
ALPINE_SOURCE := $(ALPINE)/source/alpine-minirootfs-$(ALPINE_VERSION)-riscv64.tar.gz
SYSROOT := $(GUEST)/build/sysroot
GENEXT2FS := $(OCIS)/genext2fs/dist/bin/genext2fs
DISKIMAGE := $(DEPS)/diskimage/build/img
BBL := $(DISKIMAGE)/bbl64.bin
KERNEL := $(DISKIMAGE)/kernel-riscv64.bin
ENGINE := $(OCIS)/engine
STAGE := $(GUEST)/build/rootfs
ROOTFS := $(GUEST)/dist/rootfs.ext2
DEVTABLE := $(GUEST)/devtable.txt
PACK_IMAGE := $(BOOT_IMAGE)
RATA := $(GUEST)/rata/build/rata
OUT ?= $(GUEST)/build/out

.DEFAULT_GOAL := test
.NOTPARALLEL:

.PHONY: test prepare boot-image tidy burn

prepare: $(SYSROOT)/lib/ld-musl-riscv64.so.1
	@test -n "$(HOUSE)" -a -n "$(HOST)" -a -n "$(DEVICE_KIND)"
	@test -n "$(RAM)" -a -n "$(FRAME)"
	@$(MAKE) -C "$(ENGINE)" up
	@$(MAKE) -C "$(ALPINE)" fetch
	@$(MAKE) -C "$(GUEST)/rata" test
	@$(MAKE) -C "$(OCIS)/genext2fs" build
	@$(MAKE) -C "$(DEPS)/diskimage" build
	@$(MAKE) -C "$(DEPS)/tinyemu" build

$(SYSROOT)/lib/ld-musl-riscv64.so.1:
	@$(MAKE) -C "$(ALPINE)" fetch
	@rm -rf "$(SYSROOT)"
	@mkdir -p "$(SYSROOT)"
	@tar -xzf "$(ALPINE_SOURCE)" -C "$(SYSROOT)"
	@test -e "$@"
	@test -e "$(SYSROOT)/usr/lib/libz.so.1"

boot-image:
	@$(MAKE) -C "$(ENGINE)" boot-image

$(ROOTFS): prepare boot-image $(DEVTABLE)
	@rm -rf "$(STAGE)"
	@mkdir -p "$(STAGE)" "$(dir $(ROOTFS))"
	@tar -xzf "$(ALPINE_SOURCE)" -C "$(STAGE)"
	@mkdir -p "$(STAGE)/bin" "$(STAGE)/sbin" "$(STAGE)/etc/init.d" "$(STAGE)/proc" "$(STAGE)/sys" "$(STAGE)/dev" "$(STAGE)/mnt/data"
	@cp "$(RATA)" "$(STAGE)/bin/rata"
	@cp "$(GUEST)/run-rata" "$(STAGE)/bin/run-rata"
	@cp "$(GUEST)/inittab" "$(STAGE)/etc/inittab"
	@cp "$(GUEST)/rcS" "$(STAGE)/etc/init.d/rcS"
	@chmod +x "$(STAGE)/bin/run-rata" "$(STAGE)/etc/init.d/rcS" "$(STAGE)/bin/rata"
	@ln -sf /bin/busybox "$(STAGE)/sbin/init"
	@test -s "$(STAGE)/etc/alpine-release"
	@find "$(STAGE)" -name .DS_Store -delete
	@test -x "$(STAGE)/bin/rata"
	@rm -f "$(ROOTFS)"
	@used=$$(du -sk "$(STAGE)" | awk '{print $$1}'); \
	 head=$$((used / 4)); \
	 if [ $$head -lt 2048 ]; then head=2048; fi; \
	 if [ $$head -gt 8192 ]; then head=8192; fi; \
	 blocks=$$(( (used + head + 4095) / 4096 * 4096 )); \
	 nodes=$$(find "$(STAGE)" | wc -l | tr -d ' '); \
	 nhead=$$((nodes / 4)); \
	 if [ $$nhead -lt 256 ]; then nhead=256; fi; \
	 if [ $$nhead -gt 2048 ]; then nhead=2048; fi; \
	 inodes=$$(( (nodes + nhead + 255) / 256 * 256 )); \
	 printf 'stage_kib=%s headroom_kib=%s blocks=%s stage_inodes=%s headroom_inodes=%s inodes=%s\n' $$used $$head $$blocks $$nodes $$nhead $$inodes > "$(GUEST)/build/rootfs.size"; \
	 $(PODMAN) run --rm \
	   -v "$(STAGE)":/rootfs:ro \
	   -v "$(DEVTABLE)":/devtable.txt:ro \
	   -v "$(GENEXT2FS)":/genext2fs:ro \
	   -v "$(dir $(ROOTFS))":/out \
	   --entrypoint /bin/sh "$(PACK_IMAGE)" -c "/genext2fs -b $$blocks -N $$inodes -d /rootfs -D /devtable.txt /out/rootfs.ext2 && e2fsck -fn /out/rootfs.ext2"
	@test -s "$(ROOTFS)"

test: $(ROOTFS)
	@mkdir -p "$(OUT)"
	@cp "$(BBL)" "$(OUT)/bbl64.bin"
	@cp "$(BBL)" "$(OUT)/bios64.bin"
	@cp "$(KERNEL)" "$(OUT)/kernel-riscv64.bin"
	@cp "$(KERNEL)" "$(OUT)/kernel.bin"
	@cp "$(KERNEL)" "$(OUT)/risc64.bin"
	@cp "$(ROOTFS)" "$(OUT)/rootfs.ext2"
	@mkdir -p "$(OCIS_REFS)"
	@cp "$(OUT)/bios64.bin" "$(OCIS_REFS)/bios64.bin"
	@cp "$(OUT)/risc64.bin" "$(OCIS_REFS)/risc64.bin"
	@cp "$(OUT)/rootfs.ext2" "$(OCIS_REFS)/rootfs.ext2"
	@test -s "$(OUT)/bios64.bin"
	@test -s "$(OUT)/risc64.bin"
	@test -s "$(OUT)/rootfs.ext2"

tidy:
	@rm -rf "$(GUEST)/build" "$(GUEST)/dist"
	@$(MAKE) -C "$(GUEST)/rata" tidy

burn: tidy
	@$(MAKE) -C "$(GUEST)/rata" burn
