DIST_DIR := $(abspath ../../../dist)
TARGET   := $(DIST_DIR)/go

.PHONY: all test tidy

all: test

test:
	mkdir -p $(DIST_DIR)
	go build -o $(TARGET) ./src
	file $(TARGET) | grep -q "Mach-O 64-bit.*arm64"
	$(TARGET)

tidy:
	rm -f $(TARGET)
