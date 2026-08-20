DIST_DIR := $(abspath ../../../dist)
TARGET   := $(DIST_DIR)/ocaml

SRC_FILES := src/type.ml src/find.ml src/bind.ml src/boot.ml src/trip.ml src/main.ml

.PHONY: all test tidy

all: test

test:
	mkdir -p $(DIST_DIR) dist/ocaml_obj
	PATH="/opt/homebrew/bin:$$PATH" ocamlopt -I dist/ocaml_obj -c src/stubs.c
	mv stubs.o dist/ocaml_obj/
	PATH="/opt/homebrew/bin:$$PATH" ocamlopt -I dist/ocaml_obj -I src -o $(TARGET) dist/ocaml_obj/stubs.o $(SRC_FILES)
	file $(TARGET) | grep -q "Mach-O 64-bit.*arm64"
	$(TARGET)

tidy:
	rm -rf dist src/*.cmi src/*.cmx src/*.o $(TARGET)
