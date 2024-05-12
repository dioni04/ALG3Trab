#ifndef __TRIE__
#define __TRIE__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 39
#define TAM_LINHA 1024
#define indC(c) (c - 'a')

typedef struct TrieNode {
    struct TrieNode* prox[MAX];
} node;

//Arquivo
node* processaArq(char *path, node* raiz);


//Trie Tree
node* criaNode();
void freeArv(node* arv);
node* insere(node* raiz, char* string);
int busca(node* raiz, char* string);

//Outras funcoes
int buscaPos(char c);
void penis(node* arv, char *string);
void buscaPadrao(node* arv, char *string, char *resultado, unsigned long i);
void buscaPrefixo(node* arv, char *prefix, char *resultado, unsigned long i);

#endif
