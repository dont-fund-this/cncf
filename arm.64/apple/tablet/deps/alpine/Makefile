ALPINE_VERSION := $(shell cat .version 2>/dev/null || echo "3.21.3")
ALPINE_MAJOR := $(shell echo $(ALPINE_VERSION) | cut -d. -f1,2)
TARBALL := source/alpine-minirootfs-$(ALPINE_VERSION)-riscv64.tar.gz
URL := https://dl-cdn.alpinelinux.org/alpine/v$(ALPINE_MAJOR)/releases/riscv64/alpine-minirootfs-$(ALPINE_VERSION)-riscv64.tar.gz

.PHONY: all fetch test build tidy burn

all: fetch

fetch: $(TARBALL)

$(TARBALL):
	@mkdir -p source
	@if [ -f ~/_pm_/alpine-minirootfs-$(ALPINE_VERSION)-riscv64.tar.gz ]; then \
		cp ~/_pm_/alpine-minirootfs-$(ALPINE_VERSION)-riscv64.tar.gz $(TARBALL); \
	else \
		curl -fsSL $(URL) -o $(TARBALL); \
	fi

test: fetch
	@test -s $(TARBALL)

build: fetch

tidy:
	@true

burn: tidy
	@rm -rf source
