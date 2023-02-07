DIRGUARD = @mkdir -p $(@D)

all: bin/main
.PHONY: bin/main
bin/main: src/*.c
	$(DIRGUARD)
	gcc -Wall -Wextra -o $@ $^