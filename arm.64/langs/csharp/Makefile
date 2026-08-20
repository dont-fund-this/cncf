DIST_DIR := $(abspath ../../../dist)
BIN_DIR  := $(DIST_DIR)/csharp_bin

.PHONY: all test tidy

all: test

test:
	mkdir -p $(BIN_DIR)
	dotnet build -c Release -o $(BIN_DIR) --verbosity quiet
	dotnet $(BIN_DIR)/csharp.dll

tidy:
	rm -rf bin obj $(BIN_DIR)
