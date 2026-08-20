DIST_DIR := $(abspath ../../../dist)
TARGET   := $(DIST_DIR)/swift

SRC_FILES := $(wildcard src/*.swift)

.PHONY: all test tidy

all: test

test:
	mkdir -p $(DIST_DIR)
	swiftc -O $(SRC_FILES) -o $(TARGET)
	file $(TARGET) | grep -q "Mach-O 64-bit.*arm64"
	$(TARGET)

tidy:
	rm -f $(TARGET)
