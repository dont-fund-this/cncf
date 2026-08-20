include $(CURDIR)/env.mk

MACHINE := $(HOUSE)
CPUS ?= 6
MEMORY ?= 6144
DISK_SIZE ?= 100

.DEFAULT_GOAL := test
.NOTPARALLEL:

.PHONY: test build boot-image boot up down tidy burn

test: boot

build: up boot-image

boot-image: $(ENGINE)/Containerfile.boot
	@rm -rf "$(ENGINE_BUILD)/boot"
	@mkdir -p "$(ENGINE_BUILD)/boot/temusrc"
	@cp $(TINYEMU)/*.c $(TINYEMU)/*.h "$(ENGINE_BUILD)/boot/temusrc/"
	@$(PODMAN) build --pull=never -t "$(BOOT_IMAGE)" -f "$(ENGINE)/Containerfile.boot" "$(ENGINE_BUILD)/boot"

boot: build
	@test -s "$(GUEST_OUT)/bbl64.bin"
	@test -s "$(GUEST_OUT)/kernel-riscv64.bin"
	@test -s "$(GUEST_OUT)/rootfs.ext2"
	@rm -rf "$(ENGINE_BUILD)/run"
	@mkdir -p "$(ENGINE_BUILD)/run/data"
	@if [ -f "$(ROOT)/shim/data/types.csv" ]; then cp "$(ROOT)/shim/data/types.csv" "$(ENGINE_BUILD)/run/data/types.csv"; else touch "$(ENGINE_BUILD)/run/data/types.csv"; fi
	@cp "$(GUEST_OUT)/bbl64.bin" "$(ENGINE_BUILD)/run/bbl64.bin"
	@cp "$(GUEST_OUT)/kernel-riscv64.bin" "$(ENGINE_BUILD)/run/kernel-riscv64.bin"
	@cp "$(GUEST_OUT)/rootfs.ext2" "$(ENGINE_BUILD)/run/rootfs.ext2"
	@printf '{ version:1, machine:"riscv64", memory_size:$(RAM), bios:"bbl64.bin", kernel:"kernel-riscv64.bin", display_device:"simplefb", width:$(FRAME_W), height:$(FRAME_H), cmdline:"console=hvc0 root=/dev/vda rw PAT_DEVICE=$(DEVICE_KIND) PAT_FB=0x300000000,$(FRAME_W)x$(FRAME_H) PAT_DOORBELL=0x200000000", drive0:{ file:"rootfs.ext2" }, fs0:{ file:"data", tag:"data" } }\n' > "$(ENGINE_BUILD)/run/boot.cfg"
	@$(PODMAN) run --rm -v "$(ENGINE_BUILD)/run:/run" "$(BOOT_IMAGE)" /bin/sh -c 'cd /run && timeout -s KILL $(BOOT_SECS) /temu -rw -m $(RAM) boot.cfg >/dev/null 2>&1; exit 0'

up:
	@if ! podman machine inspect "$(MACHINE)" >/dev/null 2>&1; then \
	  echo "Initializing Podman machine $(MACHINE)..."; \
	  podman machine init --cpus $(CPUS) --memory $(MEMORY) --disk-size $(DISK_SIZE) "$(MACHINE)"; \
	fi
	@if [ "$$(podman machine inspect "$(MACHINE)" --format '{{.State}}' 2>/dev/null)" != "running" ]; then \
	  running=$$(podman machine list --format '{{.Running}} {{.Name}}' 2>/dev/null | grep '^true' | awk '{print $$2}' | tr -d '*'); \
	  if [ -n "$$running" ] && [ "$$running" != "$(MACHINE)" ]; then \
	    echo "Stopping running machine $$running..."; \
	    podman machine stop "$$running"; \
	  fi; \
	  echo "Starting Podman machine $(MACHINE)..."; \
	  podman machine start "$(MACHINE)"; \
	fi
	@$(PODMAN) info >/dev/null 2>&1 || { \
	  echo "Waiting for Podman connection $(MACHINE)..."; \
	  sleep 3; \
	  $(PODMAN) info >/dev/null 2>&1 || { \
	    printf 'unavailable local podman connection: %s\n' "$(MACHINE)" >&2; \
	    podman machine list >&2; \
	    podman system connection list >&2; \
	    exit 1; \
	  }; \
	}
	@for image in docker.io/library/debian:bookworm docker.io/library/rust:1.88.0-slim-bookworm; do \
	  if ! $(PODMAN) image exists "$$image" >/dev/null 2>&1; then \
	    echo "Pulling $$image into $(MACHINE)..."; \
	    $(PODMAN) pull "$$image"; \
	  fi; \
	done

down:
	@if podman machine inspect "$(MACHINE)" >/dev/null 2>&1; then [ "$$(podman machine inspect "$(MACHINE)" --format '{{.State}}' 2>/dev/null)" != running ] || podman machine stop "$(MACHINE)"; fi

tidy: down
	@rm -rf "$(ENGINE_BUILD)"

burn: tidy
	@$(PODMAN) image rm -f "$(BOOT_IMAGE)" >/dev/null 2>&1 || true
	@podman machine inspect "$(MACHINE)" >/dev/null 2>&1 && podman machine rm -f "$(MACHINE)" || :
