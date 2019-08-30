cc = gcc

.PHONY: all clean

v1: ./main-v1.c
	$(cc) $^ -o $@

v3: ./main-v2.c
	$(cc) $^ -o $@

v3: ./main-v3.c
	$(cc) $^ -o $@ 

test1: v1
	time = time for i in {1..10}; do ./$^; done
	echo $((time / 10))

test2: v2
	time = time for i in {1..10}; do ./$^; done
	echo $((time / 10))

test3: v3
	time = time for i in {1..10}; do ./$^; done
	echo $((time / 10))

all: v1 v2 v3

clean:
	-rm v*
