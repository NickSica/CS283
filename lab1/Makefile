cc = gcc

.PHONY: q1.o q2.o q3.o q4.o q5.o all clean

q1.o: ./q1/main.c
	$(cc) $^ -o $@ 

q2.o: ./q2/main.c 
	$(cc) $^ -o $@

q3.o: ./q3/main.c ./q3/Sort.c
	$(cc) $^ -o $@ 

q4.o: ./q4/main.c ./q4/LinkedList.c
	$(cc) -g $^ -o $@ 

q5.o: ./q5/main.c ./q5/Array.c
	$(cc) -g $^ -o $@

all: q1.o q2.o q3.o q4.o q5.o

clean:
	-rm *.o
