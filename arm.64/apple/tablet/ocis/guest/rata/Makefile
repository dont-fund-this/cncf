include $(CURDIR)/env.mk

.DEFAULT_GOAL := test
.NOTPARALLEL:

.PHONY: test tidy

test:
	@rm -rf "$(RATA_BUILD)"
	@mkdir -p "$(RATA_INIT_SOURCE)"
	@cp "$(GUEST_RATA)/Cargo.toml" "$(GUEST_RATA)/Cargo.lock" "$(RATA_INIT_SOURCE)/"
	@cp -R "$(GUEST_RATA)/src" "$(RATA_INIT_SOURCE)/src"
	@cp -R "$(RATA_RATATUI_SOURCE)" "$(RATA_INIT_SOURCE)/ratatui"
	@cp -R "$(GUEST_SYSROOT)" "$(RATA_INIT)/sysroot"
	@$(PODMAN) build --pull=never -t "$(RATA_IMAGE)" -f "$(GUEST_RATA)/Containerfile" "$(RATA_INIT)"
	@$(PODMAN) run --rm -v "$(RATA_BUILD)":/out --entrypoint /bin/cp "$(RATA_IMAGE)" /rata /out/rata

tidy:
	@rm -rf build
