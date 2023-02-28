DIRGUARD = @mkdir -p $(@D)

all: bin/simplecomputer
.PHONY: bin/simplecomputer
bin/simplecomputer: src/main.c lib/bc.a lib/msc.a lib/readkey.a lib/tui.a lib/term.a
	$(DIRGUARD)
	gcc -Wall -Wextra -I src -o $@ $^ -lm

lib/%.a: obj/%.o
	$(DIRGUARD)
	ar rcs $@ $<

obj/%.o: src/%.c
	$(DIRGUARD)
	gcc -Wall -Wextra -c -o $@ $<

test: bin/test
.PHONY: bin/test
bin/test: test/*.c lib/bc.a lib/msc.a lib/readkey.a lib/tui.a lib/term.a
	$(DIRGUARD)
	gcc -Wall -Wextra -I src -MMD -I thirdparty -o $@ $^ -lm