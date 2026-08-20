include $(CURDIR)/env.mk

.PHONY: test build tidy burn

test: build
	@for lib in $(JAM_LIB) $(AUI_LIB) $(BOX_LIB) $(EFS_LIB) $(FOG_LIB) $(GUI_LIB) $(LUA_LIB) $(SQL_LIB) $(TUI_LIB); do \
		if [ -d "$$lib" ]; then $(MAKE) -C "$$lib" test; fi \
	done

build: system
	@for lib in $(JAM_LIB) $(AUI_LIB) $(BOX_LIB) $(EFS_LIB) $(FOG_LIB) $(GUI_LIB) $(LUA_LIB) $(SQL_LIB) $(TUI_LIB); do \
		if [ -d "$$lib" ]; then $(MAKE) -C "$$lib" build; fi \
	done

tidy:
	@for lib in $(JAM_LIB) $(AUI_LIB) $(BOX_LIB) $(EFS_LIB) $(FOG_LIB) $(GUI_LIB) $(LUA_LIB) $(SQL_LIB) $(TUI_LIB); do \
		if [ -d "$$lib" ]; then $(MAKE) -C "$$lib" $(LIB_CLEAN_TARGET); fi \
	done
	@if test -e "$(SYS)"; then chmod -R u+w "$(SYS)"; fi
	@if test -e "$(LIB_DIST)"; then chmod -R u+w "$(LIB_DIST)"; fi
	@rm -rf "$(SYS)" "$(LIB_DIST)"

burn: tidy
