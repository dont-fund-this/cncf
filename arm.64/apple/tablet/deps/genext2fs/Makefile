VERSION := 1.6.2
TARBALL := source/genext2fs-v$(VERSION).tar.gz
UPSTREAM_URL := https://downloads.sourceforge.net/project/genext2fs/genext2fs/1.4.1/genext2fs-1.4.1.tar.gz

.PHONY: all fetch test build tidy burn

all: fetch

fetch: $(TARBALL)

$(TARBALL):
	@mkdir -p source build/tmp
	@if [ -f ~/_pm_/genext2fs-v$(VERSION).tar.gz ]; then \
		cp ~/_pm_/genext2fs-v$(VERSION).tar.gz $(TARBALL); \
	else \
		curl -fsSL $(UPSTREAM_URL) -o build/tmp/genext2fs.tar.gz && \
		tar -xzf build/tmp/genext2fs.tar.gz -C build/tmp && \
		tar -czf $(TARBALL) -C build/tmp/genext2fs-1.4.1 genext2fs.c && \
		rm -rf build/tmp; \
	fi
	@test -s $(TARBALL)

test: fetch
	@test -s $(TARBALL)

build: fetch

tidy:
	@true

burn: tidy
	@rm -rf source build
