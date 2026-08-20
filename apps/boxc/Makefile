include env.mk

SRCS := $(shell find ./src -name "*.cpp")
INCLUDES := -I./src -I$(DEPS_DIR)/tinyemu/dist/include
LIBS := $(DEPS_DIR)/tinyemu/dist/lib/libtinyemu.a

.PHONY: test tidy

test:
	@$(MAKE) -C $(DEPS_DIR)/tinyemu build >/dev/null
	mkdir -p $(DIST_DIR)
	c++ -std=c++17 -O2 $(INCLUDES) \
		-Wl,-dead_strip \
		-Wl,-exported_symbol,_More \
		-Wl,-exported_symbol,_Pump \
		-Wl,-exported_symbol,_Less \
		-Wl,-exported_symbol,_box_fb \
		$(SRCS) $(LIBS) -o $(DIST_DIR)/$(BINARY_NAME)
	file $(DIST_DIR)/$(BINARY_NAME) | grep -q "Mach-O 64-bit.*arm64"
	$(DIST_DIR)/$(BINARY_NAME)

tidy:
	rm -f $(DIST_DIR)/$(BINARY_NAME)
