DIST_DIR := $(abspath ../../dist)
TARGET   := $(DIST_DIR)/cef_sample

.PHONY: all test tidy

all: test

test:
	mkdir -p $(DIST_DIR)
	c++ -std=c++20 -O2 -I./src -o $(TARGET) src/bind.cpp src/boot.cpp src/find.cpp src/main.cpp src/trip.cpp -ldl
	file $(TARGET) | grep -q "Mach-O 64-bit.*arm64"
	$(TARGET)

tidy:
	rm -f $(TARGET)
