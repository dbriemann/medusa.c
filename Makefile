.PHONY: medusa
medusa:
	clang -o bin/medusa main.c fen.c minboard.c

.PHONY: test
test:
	clang -o bin/test test/main.c munit/munit.c fen.c minboard.c
	./bin/test

clean:
	rm bin/*

all: medusa
