include $(CURDIR)/env.mk

.DEFAULT_GOAL := test
.NOTPARALLEL:

.PHONY: test build image tidy burn

test: build

build: image
	@mkdir -p "$(DIST)"
	@$(PODMAN) run --rm --entrypoint /bin/sh -v "$(DIST)":/out "$(IMAGE)" -c 'cp /genext2fs /out/genext2fs'

image:
	@$(MAKE) -C "$(DEP)" test
	@$(MAKE) -C "$(ENGINE)" up
	@rm -rf "$(BUILD)"
	@mkdir -p "$(BUILD)/src"
	@tar -xzf "$(SOURCE)" -C "$(BUILD)/src"
	@$(PODMAN) build --pull=never -t "$(IMAGE)" -f "$(CONTAINERFILE)" "$(BUILD)"

tidy:
	@rm -rf "$(BUILD)" "$(DIST)"

burn: tidy
	@$(PODMAN) image rm -f "$(IMAGE)" >/dev/null 2>&1 || true
