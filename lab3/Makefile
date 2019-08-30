cc = gcc

.PHONY: all clean

v1: ./main_v1.c
	$(cc) $^ -lpthread -lm -o $@

v2: ./main_v2.c
	$(cc) $^ -lpthread -lm -o $@

v3: ./main_v3.c
	$(cc) $^ -lpthread -lm -o $@ 

test1: v1
	./v1

test2: v2
	./v2

test3: v3
	./v3

all: v1 v2 v3

clean:
	-rm v*
