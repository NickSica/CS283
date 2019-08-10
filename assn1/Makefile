cc = gcc

.PHONY: build clean

build: ./*.c
	$(cc) $^ -pthread -o database.out

clean:
	-rm *.out
