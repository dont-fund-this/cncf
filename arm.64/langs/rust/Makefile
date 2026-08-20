DIST_DIR := $(abspath ../../../dist)
TARGET   := $(DIST_DIR)/rust

.PHONY: all test tidy

all: test

test:
	mkdir -p $(DIST_DIR)
	cargo build --release --quiet
	cp -f target/release/rust $(TARGET)
	file $(TARGET) | grep -q "Mach-O 64-bit.*arm64"
	$(TARGET)

tidy:
	cargo clean --quiet 2>/dev/null || true
	rm -rf target $(TARGET)
