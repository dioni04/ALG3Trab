#include "trie.h"

node* processaArq(char *path, node* raiz) {

    FILE* arq;
    char linha[TAM_LINHA];

    //Se nao conseguir abrir o arquivo, retorna 0
    if ((arq = fopen(path, "r")) == NULL)
        return NULL;

    while (fgets(linha, TAM_LINHA, arq) != NULL)
        raiz = insere(raiz,linha);

    fclose(arq);
    return raiz;

}

node* criaNode() {
    
    node* novo = (node*) calloc (1, sizeof(node));
    
    for (int i = 0; i < MAX; i++)
        novo->prox[i] = NULL;
    
    return novo;
}

node* insere(node* raiz, char* string) {
    
    node* aux = raiz;

    for (int i = 0; string[i] != '\0'; i++) {  //Loop eh executado ate o fim da palavra
        int pos = buscaPos(string[i]);  //Calcula a posicao da letra no vetor
        if (aux->prox[pos] == NULL)
            aux->prox[pos] = criaNode();
        aux = aux->prox[pos];  //Avanca p/ o prox nivel
    }

    aux->prox[26] = criaNode();  //O ponteiro que representa '\0' sera diferente de nulo ao fim de cada palavra

    return raiz;
}

int busca(node* raiz, char* string) {
    
    node* aux = raiz;

    for (int i = 0; string[i] != '\0'; i++) {
        int pos = buscaPos(string[i]);
        if (aux->prox[pos] == NULL) //Se a posicao esta vazia retorna 0
            return 0;
        aux = aux->prox[pos];
    }

    if ((aux != NULL) && (aux->prox[26] != NULL))
        return 1;
    return 0;
}

void freeArv(node* arv) {
    
    if (arv == NULL)
        return;

    for(int i = 0; i < MAX; i++) 
        if (arv->prox[i] != NULL)
            freeArv(arv->prox[i]);
    
    free(arv);
}

//Retorna a posicao correta do char no vetor
int buscaPos(char c) {

    if (c == ' ')
        return 37;
    else if (c == '?')
        return 36;
    else if ( c >= '0' && c <= '9' )
        return 26 + c;
    else
        return indC(c);

}
