.PHONY: all test tidy

all: test

test:
	mkdir -p bin
	PATH="/opt/homebrew/bin:$$PATH" kotlinc -d bin src/*.kt
	PATH="/opt/homebrew/bin:$$PATH" kotlin -Djava.lang.foreign.enableNativeAccess=ALL-UNNAMED -cp bin pat.MainKt

tidy:
	rm -rf bin
