DIRGUARD = @mkdir -p $(@D)

all: bin/simplecomputer
.PHONY: bin/simplecomputer
bin/simplecomputer: src/*.c
	$(DIRGUARD)
	gcc -Wall -Wextra -I src -o $@ $^

test: bin/test
.PHONY: bin/test
bin/test: test/*.c
	$(DIRGUARD)
	gcc -Wall -Wextra -I src -I thirdparty -o $@ $<