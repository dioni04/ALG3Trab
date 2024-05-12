#include "trie.h"

//Abre o arquivo de dados, e copia suas informacoes p/ a arv
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

//Cria um no com MAX filhos NULL
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
        if ((pos != -1) && (aux->prox[pos] == NULL))
            aux->prox[pos] = criaNode();
        else if (pos == -1) {
            printf("Erro: Caracter invalido\n");
            return NULL;
        }
        aux = aux->prox[pos];  //Avanca p/ o prox nivel
    }

    aux->prox[MAX-1] = criaNode();  //O ponteiro que representa '\0' sera diferente de nulo ao fim de cada palavra

    return raiz;
}

int busca(node* raiz, char* string) {
    
    node* aux = raiz;

    for (int i = 0; string[i] != '\0'; i++) {
        int pos = buscaPos(string[i]);
        if ((pos == -1) || (aux->prox[pos] == NULL)) //Se a posicao esta vazia retorna 0
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

//Retorna a posicao correta do char no vetor, caso nao encontre, retorna -1
int buscaPos(char c) {

    if (c == ' ')
        return 37;
    else if (c == '?')
        return 36;
    else if ( c >= '0' && c <= '9' )
        return 26 + c;
    else if (c >= 97 && c <= 122)
        return indC(c);
    else
        return -1;

}

//Retorna um caracter baseado na posicao do vetor, oposto da funcao buscaPos
char buscaChar(int pos) {

    if (pos == 37)
        return ' ';
    else if (pos == 36)
        return '?';
    else if (pos <= 25)
        return pos + 'a';
    else if (pos >= 26 && pos <= 35)
        return pos - 26;
    else
        return -1;
        
}

//Essa funcao chama as funcoes adequadas baseado no op
void penis(node* arv, char *string, char *op) {

    char res[TAM_LINHA]; //String para armazenar o resultado
    memset(res, '\0', sizeof(res)); //Preenche o vetor com \0 pra n dar merda
    
    if (strcmp(op,"p") == 0)
        buscaPrefixo(arv,string,res,0);
    else if (strcmp(op,"l") == 0)
        return;  //Placeholder para a funcao buscaMaisLongo
    else if (strcmp(op,"c") == 0)
        buscaPadrao(arv,string,res,0);
    else
        printf("Operacao invalida\n");
    
}

//No momento ela so busca com o wildcard '.', mas dps eh so adpatar pra funcionar pra '*' tbm
void buscaPadrao(node* arv, char *padrao, char *res, unsigned long i) {  
    
    if ((arv == NULL) || (i >= strlen(padrao))) {
        printf("%s\n", res);
        return;
    }

    int pos = buscaPos(padrao[i]);
    if (padrao[i] == '.') {
        for (int j = 0; j < MAX; j++)
            if (arv->prox[j] != NULL) {
                res[i] = buscaChar(j);
		buscaPadrao(arv->prox[j],padrao,res,i+1);
	    }
    }
    else if ((pos != -1) && (arv->prox[pos] != NULL)) {
    	res[i] = buscaChar(pos);
        buscaPadrao(arv->prox[pos],padrao,res,i+1);
    }
    else if (pos == -1) {
        printf("Erro: Caracter invalido\n");
        return;
    }

}

//Busca um titulo baseado num prefixo, na teoria, pq eu n testei 
void buscaPrefixo(node* arv, char *prefix, char *res, unsigned long i) {

	if ((arv == NULL) || (i >= strlen(prefix))) {
		printf("%s\n", res);
		return;
	}

    if (i < strlen(prefix)) {
        int pos = buscaPos(prefix[i]);
        if ((pos != -1) && (arv->prox[pos] != NULL)) {
            res[i] = buscaChar(pos);
            buscaPrefixo(arv->prox[pos],prefix,res,i+1);
        }
    }
    else
        for (int j = 0; j < MAX; j++)
            if (arv->prox[j] != NULL) {
                res[i] = buscaPos(j);
                buscaPrefixo(arv->prox[j],prefix,res,i+1);
            }
}
