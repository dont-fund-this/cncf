.PHONY: all test tidy

all: test

test:
	NODE_PATH=$(abspath ../node/node_modules) npx --prefix $(abspath ../node) tsx src/main.ts

tidy:
	rm -rf node_modules
