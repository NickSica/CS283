cc = gcc
files = csapp.c miniRSA.c messageFunctions.c

.PHONY: clean

cbuild: client.c $(files)
	$(cc) $^ -lpthread -lm -g -o client

sbuild: server.c $(files)
	$(cc) $^ -lpthread -lm -g -o server

miniRSA: rsa.c miniRSA.c
	$(cc) $^ -lpthread -lm -g -o $@

all: cbuild sbuild miniRSA

genkey: miniRSA
	./$^ $@ "${MPRIME}" "${NPRIME}"

keycrack: miniRSA
	./$^ $@ "${E}" "${C}"

encrypt: miniRSA
	./$^ $@ "${E}" "${C}"

client: cbuild
	./$@ "${SERVER}" "${PORT}" "${E}" "${C}" "${D}" "${DC}"

server: sbuild
	./$@ "${PORT}" "${E}" "${C}" "${D}" "${DC}" 

clean:
	-rm server client miniRSA genkey keycrack encrypt
