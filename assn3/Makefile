cc = gcc

.PHONY: clean

build: main.c
	$(cc) $^ -lpthread -lm -g -o game

run: build
	./game "${GAMES}" "${DIMENSION}"

clean:
	-rm game
