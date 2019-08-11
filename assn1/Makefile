cc = gcc

.PHONY: build clean

build: ./*.c
	$(cc) $^ -pthread -g -o database.out

test: build
	./database.out test

clean:
	-rm *.out
