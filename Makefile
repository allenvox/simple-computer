DIRGUARD = @mkdir -p $(@D)

all: bin/simplecomputer
.PHONY: bin/simplecomputer
bin/simplecomputer: src/main.c bin/lib.a
	$(DIRGUARD)
	gcc -Wall -Wextra -lm -I src -o $@ $^

bin/lib.a: obj/bc.o obj/msc.o obj/term.o obj/term_gui.o
	$(DIRGUARD)
	ar rcs $@ $^

obj/%.o: src/%.c
	$(DIRGUARD)
	gcc -Wall -Wextra -lm -c -o $@ $<

test: bin/test
.PHONY: bin/test
bin/test: test/*.c bin/lib.a
	$(DIRGUARD)
	gcc -Wall -Wextra -lm -I src -MMD -I thirdparty -o $@ $^