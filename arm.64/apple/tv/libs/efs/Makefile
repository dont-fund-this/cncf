include $(CURDIR)/env.mk

OUT := $(LIB_DIST)/libefs
PACK := build/pack
PACK_TMP := $(abspath build/tmp)

.PHONY: tidy build tool archive test

build: $(OUT)

$(OUT): lib.rs $(SYS)/liballoc.rlib
	@mkdir -p "$(dir $(OUT))"
	@$(RUSTC) --edition 2021 --crate-type=cdylib $(SYS_ARGS) \
	  -Clink-arg=-lSystem -Clink-arg=-lz \
	  -Clink-arg=-Wl,-install_name,@rpath/libefs.framework/libefs \
	  -C strip=symbols -Clink-arg=-Wl,-segalign,0x4000 -o "$(OUT)" lib.rs

tool: $(PACK)

$(PACK): pack/main.rs
	@mkdir -p "$(dir $(PACK))" "$(PACK_TMP)"
	@TMPDIR="$(PACK_TMP)" RUSTFLAGS="" rustc --edition 2021 -C opt-level=z -C lto -C codegen-units=1 \
	  -C panic=abort -Clink-arg=-lSystem -Clink-arg=-lz pack/main.rs -o "$(PACK)"

archive: tool
	@test -n "$(PACK_ROOT)"
	@test -n "$(PACK_INPUT)"
	@test -n "$(PACK_OUTPUT)"
	@ROOT="$(PACK_ROOT)" STATS_OUTPUT="$(STATS_OUTPUT)" "$(PACK)" "$(PACK_OUTPUT)" < "$(PACK_INPUT)"

test: build
	@test -s "$(OUT)"

tidy:
	@rm -rf build
	@rm -f "$(OUT)"
