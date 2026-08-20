DIST_DIR := $(abspath ../../../dist)
TARGET   := $(DIST_DIR)/zig

.PHONY: all test tidy

all: test

test:
	mkdir -p $(DIST_DIR)
	PATH="/opt/homebrew/bin:$$PATH" zig build-exe -target aarch64-macos -O ReleaseSafe src/main.zig -femit-bin=$(TARGET)
	file $(TARGET) | grep -q "Mach-O 64-bit.*arm64"
	$(TARGET)

tidy:
	rm -f $(TARGET)
