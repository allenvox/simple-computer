DIRGUARD = @mkdir -p $(@D)

all: bin/simplecomputer
.PHONY: bin/simplecomputer
bin/simplecomputer: src/*.c
	$(DIRGUARD)
	gcc -Wall -Wextra -o $@ $^