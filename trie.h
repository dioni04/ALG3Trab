#ifndef __TRIE__
#define __TRIE__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 27
#define indC(c) (c - 'a')

typedef struct TrieNode {
    struct TrieNode* prox[MAX];
} node;

node* criaNode();
void freeArv(node* arv);
node* insere(node* raiz, char* string);
int busca(node* raiz, char* string);

#endif
