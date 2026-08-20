.PHONY: all test tidy

all: test

test:
	mkdir -p $(abspath ../../dist)
	cargo build --release --quiet
	cp -f target/release/slint_sample $(abspath ../../dist)/slint_sample
	file $(abspath ../../dist)/slint_sample | grep -q "Mach-O 64-bit.*arm64"
	PAT_TEST=1 $(abspath ../../dist)/slint_sample

tidy:
	cargo clean --quiet 2>/dev/null || true
	rm -rf target $(abspath ../../dist)/slint_sample
