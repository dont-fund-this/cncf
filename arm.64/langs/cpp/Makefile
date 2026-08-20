DIST_DIR := $(abspath ../../../dist)
TARGET   := $(DIST_DIR)/cpp
SRC_FILES := $(wildcard src/*.cpp)

.PHONY: all test tidy

all: test

test:
	mkdir -p $(DIST_DIR)
	c++ -std=c++20 -O2 -I./src -I$(abspath ../../../deps/json/include) -o $(TARGET) $(SRC_FILES) -ldl
	file $(TARGET) | grep -q "Mach-O 64-bit.*arm64"
	$(TARGET)

tidy:
	rm -f $(TARGET)
