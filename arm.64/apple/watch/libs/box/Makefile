include $(CURDIR)/env.mk

OUT := $(LIB_DIST)/libbox
TINYEMU := build/libtinyemu.a

CC := xcrun -sdk $(SDK) clang -target $(TRIPLE)
AR := xcrun -sdk $(SDK) ar
DEFS := -DCONFIG_VERSION=\"2019-12-21\" -D_GNU_SOURCE -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE -DCONFIG_RISCV_MAX_XLEN=64 -DMAX_XLEN=64
INCLUDES := -I$(CURDIR)/compat -I$(DEPS)/tinyemu

TEMU_SRCS := virtio.c pci.c fs.c fs_utils.c cutils.c iomem.c simplefb.c json.c machine.c riscv_machine.c softfp.c
TEMU_OBJS := $(addprefix build/,$(TEMU_SRCS:.c=.o) riscv_cpu64.o riscv_cpu32.o)

.PHONY: tidy build test

build: $(OUT)

$(TINYEMU): $(TEMU_OBJS)
	@mkdir -p $(dir $@)
	$(AR) rcs $@ $(TEMU_OBJS)

build/%.o: $(DEPS)/tinyemu/%.c Makefile env.mk
	@mkdir -p build
	$(CC) -O2 -w $(DEFS) $(INCLUDES) -c $< -o $@

build/riscv_cpu64.o: $(DEPS)/tinyemu/riscv_cpu.c Makefile env.mk
	@mkdir -p build
	$(CC) -O2 -w $(DEFS) -DMAX_XLEN=64 $(INCLUDES) -c $< -o $@

build/riscv_cpu32.o: $(DEPS)/tinyemu/riscv_cpu.c Makefile env.mk
	@mkdir -p build
	$(CC) -O2 -w $(DEFS) -DMAX_XLEN=32 $(INCLUDES) -c $< -o $@

$(OUT): lib.rs $(SYS)/liballoc.rlib $(TINYEMU)
	@mkdir -p "$(dir $(OUT))"
	@$(RUSTC) --edition 2021 --crate-type=cdylib $(SYS_ARGS) \
	  -Clink-arg=-lSystem -Clink-arg=-L$(CURDIR)/build -Clink-arg=-ltinyemu -Clink-arg=-lz \
	  -Clink-arg=-Wl,-U,_rust_eh_personality \
	  -Clink-arg=-Wl,-install_name,@rpath/libbox.framework/libbox \
	  -C strip=symbols -Clink-arg=-Wl,-segalign,0x4000 -o "$(OUT)" lib.rs

test: build
	@test -s "$(OUT)"

tidy:
	@rm -f "$(OUT)"
	@rm -rf build
