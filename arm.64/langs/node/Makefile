.PHONY: all test tidy

all: test

test:
	@[ -d node_modules ] || npm install --silent
	npx tsx src/main.ts

tidy:
	rm -rf node_modules
