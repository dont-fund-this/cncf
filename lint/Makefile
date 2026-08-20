include env.mk

INCLUDES := -I../deps/tree-sitter/lib/include \
            -I../deps/tree-sitter-rust/src \
            -I../deps/tree-sitter-cpp/src \
            -I../deps/tree-sitter-go/src

.PHONY: test tidy

test:
	@rm -rf bin
	@mkdir -p bin
	@cc -c -O3 $(INCLUDES) ../deps/tree-sitter/lib/src/lib.c -o bin/ts_lib.o
	@cc -c -O3 $(INCLUDES) ../deps/tree-sitter-rust/src/parser.c -o bin/rust_parser.o
	@cc -c -O3 $(INCLUDES) ../deps/tree-sitter-rust/src/scanner.c -o bin/rust_scanner.o
	@cc -c -O3 $(INCLUDES) ../deps/tree-sitter-cpp/src/parser.c -o bin/cpp_parser.o
	@cc -c -O3 $(INCLUDES) ../deps/tree-sitter-cpp/src/scanner.c -o bin/cpp_scanner.o
	@cc -c -O3 $(INCLUDES) ../deps/tree-sitter-go/src/parser.c -o bin/go_parser.o
	@c++ -std=c++17 $(INCLUDES) ./src/main.cpp bin/*.o -o $(LINT_BIN)
	@$(LINT_BIN) ../apps/fogr/src ../apps/fogc/src ../apps/fogg/src

tidy:
	rm -rf bin
