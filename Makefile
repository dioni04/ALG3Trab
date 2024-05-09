parametrosCompilacao=-Wall -g -lm #-Wshadow
nomePrograma=qualFilme

all: $(nomePrograma)

$(nomePrograma): qualFilme.o trie.o
	gcc -o $(nomePrograma) qualFilme.o trie.o $(parametrosCompilacao)

qualFilme.o: qualFilme.c
	gcc -c qualFilme.c $(parametrosCompilacao)

trie.o:  trie.c trie.h
	gcc -c trie.c $(parametrosCompilacao)

clean:
	rm -f *.o *.gch

purge:
	rm -f *.o *.gch $(nomePrograma)
