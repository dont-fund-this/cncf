include env.mk

SRCS := $(shell find ./src -name "*.cpp")
INCLUDES := -I./src/sql -I./src -I$(DEPS_DIR)/json/include -I$(DEPS_DIR)/sqlite/dist/include
LIBS := $(DEPS_DIR)/sqlite/dist/lib/libsqlite3.a

.PHONY: test tidy

test:
	mkdir -p $(DIST_DIR)
	c++ -std=c++20 -O2 $(INCLUDES) \
		-Wl,-dead_strip \
		-Wl,-exported_symbol,_More \
		-Wl,-exported_symbol,_Pump \
		-Wl,-exported_symbol,_Less \
		$(SRCS) $(LIBS) -o $(DIST_DIR)/$(BINARY_NAME)
	file $(DIST_DIR)/$(BINARY_NAME) | grep -q "Mach-O 64-bit.*arm64"
	$(DIST_DIR)/$(BINARY_NAME)

tidy:
	rm -f $(DIST_DIR)/$(BINARY_NAME)
