SOURCE := source
REV := v0.29.0
URL := https://github.com/ratatui/ratatui.git

.PHONY: all fetch test build tidy burn

all: fetch

fetch: $(SOURCE)

$(SOURCE):
	@mkdir -p $(SOURCE)
	@if [ -d ~/_pm_/ratatui ]; then \
		cp -r ~/_pm_/ratatui/* $(SOURCE)/; \
	else \
		git clone --depth 1 --branch $(REV) $(URL) $(SOURCE); \
	fi

test: fetch
	@test -d $(SOURCE)

build: fetch

tidy:
	@true

burn: tidy
	@rm -rf $(SOURCE)
