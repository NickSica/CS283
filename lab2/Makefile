cc = gcc

.PHONY: $(client) $(server) ctest stest clean

part1: part1.c csapp.c
	$(cc) $^ -pthread -g -o part1.out

tpart1: part1
	./part1.out google.com /index.html 

client: client.c csapp.c
	$(cc) $^ -pthread -g -o client.out

server: server.c csapp.c
	$(cc) $^ -pthread -g -o server.out

tserver: server
	./server.out 13000

tclient: client
	./client.out localhost 13000

tpart2: server client
	./server.out 13000 &
	./client.out localhost 13000

clean:
	-rm *.out
