DIST_DIR := $(abspath ../../dist)
TARGET   := $(DIST_DIR)/metal_sample

.PHONY: all test tidy

all: test

test:
	mkdir -p $(DIST_DIR)
	swiftc -O src/bind.swift src/boot.swift src/find.swift src/main.swift src/trip.swift src/type.swift -o $(TARGET)
	file $(TARGET) | grep -q "Mach-O 64-bit.*arm64"
	$(TARGET)

tidy:
	rm -f $(TARGET)
