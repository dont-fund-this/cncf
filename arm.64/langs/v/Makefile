DIST_DIR := $(abspath ../../../dist)
TARGET   := $(DIST_DIR)/v

.PHONY: all test tidy

all: test

test:
	mkdir -p $(DIST_DIR)
	PATH="/opt/homebrew/bin:$$PATH" v -o $(TARGET) src/
	file $(TARGET) | grep -q "Mach-O 64-bit.*arm64"
	$(TARGET)

tidy:
	rm -f $(TARGET)
