all: main
.PHONY: main
main: *.c
	gcc -Wall -Wextra $@ $^

