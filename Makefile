DIRGUARD = @mkdir -p $(@D)

all: bin/simplecomputer
.PHONY: bin/simplecomputer
bin/simplecomputer: src/main.c bin/lib.a
	$(DIRGUARD)
	gcc -Wall -Wextra -I src -o $@ $^ -lm

bin/lib.a: obj/bc.o obj/msc.o obj/term.o obj/term_gui.o
	$(DIRGUARD)
	ar rcs $@ $^

obj/%.o: src/%.c
	$(DIRGUARD)
	gcc -Wall -Wextra -c -o $@ $< -lm

test: bin/test
.PHONY: bin/test
bin/test: test/*.c bin/lib.a
	$(DIRGUARD)
	gcc -Wall -Wextra -I src -MMD -I thirdparty -o $@ $^ -lm