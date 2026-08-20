include env.mk

.PHONY: test tidy

test:
	$(MAKE) -C boxc test
	$(MAKE) -C efsc test
	$(MAKE) -C fogc test
	$(MAKE) -C fogg test
	$(MAKE) -C fogr test
	$(MAKE) -C rata test
	$(MAKE) -C sqlc test

tidy:
	$(MAKE) -C boxc tidy
	$(MAKE) -C efsc tidy
	$(MAKE) -C fogc tidy
	$(MAKE) -C fogg tidy
	$(MAKE) -C fogr tidy
	$(MAKE) -C rata tidy
	$(MAKE) -C sqlc tidy
