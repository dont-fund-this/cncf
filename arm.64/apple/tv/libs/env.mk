include $(abspath $(dir $(lastword $(MAKEFILE_LIST)))/../env.mk)

.PHONY: system

system: $(SYS)/liballoc.rlib

$(SYS)/liballoc.rlib:
	@test -d "$(RUSTLIB)"
	@mkdir -p "$(SYS)"
	@$(RUSTC) --edition 2024 --crate-type=rlib --crate-name core "$(RUSTLIB)/core/src/lib.rs" --out-dir "$(SYS)"
	@$(RUSTC) --edition 2024 --crate-type=rlib --crate-name compiler_builtins --cfg 'feature="compiler-builtins"' --cfg 'feature="mem"' --extern core="$(SYS)/libcore.rlib" "$(RUSTLIB)/compiler-builtins/compiler-builtins/src/lib.rs" --out-dir "$(SYS)"
	@$(RUSTC) --edition 2024 --crate-type=rlib --crate-name alloc -Zforce-unstable-if-unmarked --extern core="$(SYS)/libcore.rlib" --extern compiler_builtins="$(SYS)/libcompiler_builtins.rlib" "$(RUSTLIB)/alloc/src/lib.rs" --out-dir "$(SYS)"

JAM_LIB := $(LIBS)/jam
AUI_LIB := $(LIBS)/aui
BOX_LIB := $(LIBS)/box
EFS_LIB := $(LIBS)/efs
FOG_LIB := $(LIBS)/fog
GUI_LIB := $(LIBS)/gui
LUA_LIB := $(LIBS)/lua
SQL_LIB := $(LIBS)/sql
TUI_LIB := $(LIBS)/tui
LIB_CLEAN_TARGET := tidy
