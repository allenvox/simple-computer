all: bin/main
.PHONY: bin/main
bin/main: src/*.c
	gcc -Wall -Wextra -o $@ $^