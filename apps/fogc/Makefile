include env.mk

SRCS := $(shell find ./src -name "*.cpp")
INCLUDES := -I./src -I$(DEPS_DIR)/json/include

.PHONY: test tidy

test:
	mkdir -p $(DIST_DIR)
	c++ -std=c++20 -Os $(INCLUDES) \
		-Wl,-dead_strip \
		-Wl,-exported_symbol,_More \
		-Wl,-exported_symbol,_Pump \
		-Wl,-exported_symbol,_Less \
		$(SRCS) -o $(DIST_DIR)/$(BINARY_NAME)
	file $(DIST_DIR)/$(BINARY_NAME) | grep -q "Mach-O 64-bit.*arm64"
	$(DIST_DIR)/$(BINARY_NAME)

tidy:
	rm -f $(DIST_DIR)/$(BINARY_NAME)
