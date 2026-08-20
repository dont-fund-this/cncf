include env.mk

.PHONY: test tidy

test:
	mkdir -p $(DIST_DIR)
	go build -ldflags="-s -w" -o $(DIST_DIR)/$(BINARY_NAME) ./src
	file $(DIST_DIR)/$(BINARY_NAME) | grep -q "Mach-O 64-bit.*arm64"
	$(DIST_DIR)/$(BINARY_NAME)

tidy:
	@command -v go >/dev/null 2>&1 && go fmt ./src/... 2>/dev/null || true
	rm -f $(DIST_DIR)/$(BINARY_NAME)
