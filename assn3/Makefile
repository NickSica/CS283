cc = gcc

.PHONY: clean

build: main.c csapp.c game.c
	$(cc) $^ -lpthread -lm -g -o game

run: build
	./game "${GAMES}" "${DIMENSION}"

test: build
	@echo "Test 1"
	./game 1 8
	@echo "Test 2"
	./game 0 8
	@echo "Test 3"
	./game 8 3
	@echo "Test 4"
	./game 8 8
	@echo "Test 5"
	./game 100 8

clean:
	-rm game
