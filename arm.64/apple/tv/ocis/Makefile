include $(CURDIR)/env.mk

.DEFAULT_GOAL := test
.NOTPARALLEL:

.PHONY: test tidy burn

test:
	@$(MAKE) -C "$(GENEXT2FS)" test
	@$(MAKE) -C "$(GUEST)" test
	@$(MAKE) -C "$(ENGINE)" test

tidy:
	@$(MAKE) -C "$(GENEXT2FS)" tidy
	@$(MAKE) -C "$(GUEST)" tidy
	@$(MAKE) -C "$(ENGINE)" tidy

burn:
	@$(MAKE) -C "$(GENEXT2FS)" burn
	@$(MAKE) -C "$(GUEST)" burn
	@$(MAKE) -C "$(ENGINE)" burn
