#ifndef __TRIE__
#define __TRIE__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX 39
#define TAM_LINHA 1024
//#define indC(c) (c - 'a')

typedef struct TrieNode {
    struct TrieNode* prox[MAX];
} node;

typedef struct MaisLongo {
    char *word;
    unsigned long tam;
} maisLongo;

//Arquivo
node* processaArq(char *path, node* raiz);

//Trie Tree
node* criaNode();
void freeArv(node* arv);
node* insere(node* raiz, char* string);
//int busca(node* raiz, char* string);

//Funcoes principais
void busca(node* arv, char *string, char* op, FILE* saida);
void buscaPadrao(node* arv, char *padrao, char *res, unsigned long i, FILE* saida);
void buscaPadraoAux(node* arv, char* padrao, char* res, int i, int proxPos, FILE* saida);
void buscaPrefixo(node* arv, char *prefix, char *res, unsigned long i, FILE* saida);
void buscaMaisLongo(node* arv, char *titulo, char *res, maisLongo *nome, unsigned long i);

//Outras funcoes
int buscaPos(char c);
char buscaChar(int pos);
char* padronizaString(char* entrada);

#endif
