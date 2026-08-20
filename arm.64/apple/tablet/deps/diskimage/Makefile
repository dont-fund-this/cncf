BUILD_DIR := build/img
BBL := $(BUILD_DIR)/bbl64.bin
KERNEL := $(BUILD_DIR)/kernel-riscv64.bin
URL := https://bellard.org/tinyemu/diskimage-linux-riscv-2018-09-23.tar.gz

.PHONY: all build test tidy burn

all: build

build: $(BBL) $(KERNEL)

$(BBL) $(KERNEL):
	@mkdir -p $(BUILD_DIR)
	@if [ -f ~/_pm_/bbl64.bin ] && [ -f ~/_pm_/kernel-riscv64.bin ]; then \
		cp ~/_pm_/bbl64.bin $(BUILD_DIR)/; \
		cp ~/_pm_/kernel-riscv64.bin $(BUILD_DIR)/; \
	elif [ -d ~/_pm_/diskimage ]; then \
		cp ~/_pm_/diskimage/bbl64.bin $(BUILD_DIR)/ 2>/dev/null || true; \
		cp ~/_pm_/diskimage/kernel-riscv64.bin $(BUILD_DIR)/ 2>/dev/null || true; \
	fi
	@if [ ! -s $(BBL) ] || [ ! -s $(KERNEL) ]; then \
		mkdir -p build/tmp && \
		curl -fsSL $(URL) -o build/tmp/diskimage.tar.gz && \
		tar -xzf build/tmp/diskimage.tar.gz -C build/tmp && \
		find build/tmp -name "bbl64.bin" -exec cp {} $(BUILD_DIR)/ \; && \
		find build/tmp -name "kernel-riscv64.bin" -exec cp {} $(BUILD_DIR)/ \; && \
		rm -rf build/tmp; \
	fi
	@test -s $(BBL)
	@test -s $(KERNEL)

test: build

tidy:
	@true

burn: tidy
	@rm -rf build
