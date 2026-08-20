include $(CURDIR)/env.mk

OUT := $(LIB_DIST)/libjam

.PHONY: tidy build test

build: $(OUT)

$(OUT): lib.rs $(SYS)/liballoc.rlib
	@mkdir -p "$(dir $(OUT))"
	@$(RUSTC) --edition 2021 --crate-type=cdylib $(SYS_ARGS) \
	  -Clink-arg=-lSystem \
	  -Clink-arg=-Wl,-install_name,@rpath/libjam.framework/libjam \
	  -C strip=symbols -Clink-arg=-Wl,-segalign,0x4000 -o "$(OUT)" lib.rs

test: build
	@test -s "$(OUT)"

tidy:
	@rm -f "$(OUT)"
