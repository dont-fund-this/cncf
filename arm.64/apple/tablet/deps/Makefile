include $(CURDIR)/env.mk

.PHONY: test tidy burn

test:
	@$(MAKE) -C $(ALPINE) test
	@$(MAKE) -C $(DISKIMAGE) test
	@$(MAKE) -C $(GENEXT2FS_DEP) test
	@$(MAKE) -C $(RATATUI) test
	@[ ! -f "$(TINYEMU)/Makefile" ] || $(MAKE) -C $(TINYEMU) test 2>/dev/null || true

tidy:
	@$(MAKE) -C $(ALPINE) tidy
	@$(MAKE) -C $(DISKIMAGE) tidy
	@$(MAKE) -C $(GENEXT2FS_DEP) tidy
	@$(MAKE) -C $(RATATUI) tidy
	@[ ! -f "$(TINYEMU)/Makefile" ] || $(MAKE) -C $(TINYEMU) tidy 2>/dev/null || true

burn:
	@$(MAKE) -C $(ALPINE) burn
	@$(MAKE) -C $(DISKIMAGE) burn
	@$(MAKE) -C $(GENEXT2FS_DEP) burn
	@$(MAKE) -C $(RATATUI) burn
	@[ ! -f "$(TINYEMU)/Makefile" ] || $(MAKE) -C $(TINYEMU) burn 2>/dev/null || true
