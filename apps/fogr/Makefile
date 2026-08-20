include env.mk

.PHONY: test tidy

test:
	mkdir -p $(DIST_DIR)
	rustc --edition=2021 \
		-C panic=abort \
		-C opt-level=z \
		-C lto=fat \
		-C codegen-units=1 \
		-C strip=symbols \
		-C link-arg=-lSystem \
		-C link-arg=-Wl,-dead_strip \
		-C link-arg=-Wl,-exported_symbol,_More \
		-C link-arg=-Wl,-exported_symbol,_Pump \
		-C link-arg=-Wl,-exported_symbol,_Less \
		./src/main.rs -o $(DIST_DIR)/$(BINARY_NAME)
	file $(DIST_DIR)/$(BINARY_NAME) | grep -q "Mach-O 64-bit.*arm64"
	test $$(stat -f%z $(DIST_DIR)/$(BINARY_NAME)) -le 65536
	$(DIST_DIR)/$(BINARY_NAME)

tidy:
	@command -v rustfmt >/dev/null 2>&1 && find ./src -name "*.rs" | xargs rustfmt 2>/dev/null || true
	rm -f $(DIST_DIR)/$(BINARY_NAME)
