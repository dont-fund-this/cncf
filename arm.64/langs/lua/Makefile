.PHONY: all test tidy

all: test

test:
	PATH="/opt/homebrew/bin:$$PATH" LUA_PATH="src/?.lua;;" luajit src/main.lua

tidy:
	@true
