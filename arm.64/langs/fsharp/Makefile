DIST_DIR := $(abspath ../../../dist)
BIN_DIR  := $(DIST_DIR)/fsharp_bin

.PHONY: all test tidy

all: test

test:
	mkdir -p $(BIN_DIR)
	dotnet publish -c Release -o $(BIN_DIR) --verbosity quiet
	dotnet $(BIN_DIR)/fsharp.dll

tidy:
	rm -rf bin obj $(BIN_DIR)
