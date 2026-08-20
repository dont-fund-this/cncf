DIST_DIR := $(abspath ../../../dist)
TARGET   := $(DIST_DIR)/haskell

.PHONY: all test tidy

all: test

test:
	mkdir -p $(DIST_DIR) dist/haskell_obj
	PATH="/opt/homebrew/bin:$$PATH" ghc -O2 -isrc -outputdir dist/haskell_obj -o $(TARGET) src/Main.hs
	file $(TARGET) | grep -q "Mach-O 64-bit.*arm64"
	$(TARGET)

tidy:
	rm -rf dist $(TARGET)
