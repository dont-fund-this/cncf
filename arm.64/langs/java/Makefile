.PHONY: all test tidy

all: test

test:
	mkdir -p bin
	javac -d bin src/*.java
	java --enable-native-access=ALL-UNNAMED -cp bin pat.main

tidy:
	rm -rf bin
