DIST_DIR := $(abspath ../../dist)
TARGET   := $(DIST_DIR)/avalonia_bin/avalonia_sample.dll

.PHONY: all test tidy

all: test

test:
	mkdir -p $(DIST_DIR)/avalonia_bin
	dotnet publish -c Release -o $(DIST_DIR)/avalonia_bin --verbosity quiet
	dotnet $(TARGET)

tidy:
	rm -rf bin obj $(DIST_DIR)/avalonia_bin
