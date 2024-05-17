#include "trie.h"

//Define indice do caracter no Nodo
int indC(char c){
    if(c == 0)
        return 0;

    if(c == ' ')
        return 1;

    if(c >= '0' && c <= '9')
        return 2 + (c - '0');

    c = tolower(c); //Transforma em caracter minusculo
    if(c >= 'a' && c <= 'z')
        return 12 + (c - 'a');

    return MAX-1; //Caracter desconhecido ?
}

//Abre o arquivo de dados, e copia suas informacoes p/ a arv
node* processaArq(char *path, node* raiz) {

    FILE* arq;
    char linha[TAM_LINHA];

    //Se nao conseguir abrir o arquivo, retorna 0
    if ((arq = fopen(path, "r")) == NULL)
        return NULL;

    while (fgets(linha, TAM_LINHA, arq) != NULL) {
		linha[strcspn(linha, "\n")] = 0;
        raiz = insere(raiz,linha);
	}

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

    if ((aux != NULL) && (aux->prox[MAX-1] != NULL))
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
void penis(node* arv, char *string, char *op, FILE* saida) {

    char res[TAM_LINHA]; //String para armazenar o resultado
    memset(res, '\0', sizeof(res)); //Preenche o vetor com \0 pra n dar merda
    
    if (strcmp(op,"p") == 0)
        buscaPrefixo(arv,string,res,0,saida);
    else if (strcmp(op,"l") == 0) {
        maisLongo nome;
        nome.tam = 0;
        nome.word = NULL;
        buscaMaisLongo(arv,string,res,&nome,0);
        fputs(nome.word,saida);
        fputs("\n",saida);
		free(nome.word);
    }
    else if (strcmp(op,"c") == 0)
        buscaPadrao(arv,string,res,0,saida);
    else
        printf("Operacao invalida\n");
    
}

//No momento ela so busca com o wildcard '.', mas dps eh so adpatar pra funcionar pra '*' tbm
void buscaPadrao(node* arv, char *padrao, char *res, unsigned long i, FILE* saida) {  
    
    if ((arv == NULL) || (i >= strlen(padrao))) {
        if (arv->prox[MAX-1] != NULL) {
            fputs(res,saida);
            fputs("\n",saida);
        }
        return;
    }

    int pos = buscaPos(padrao[i]);
    if (padrao[i] == '.') {
        for (int j = 0; j < MAX; j++)
            if (arv->prox[j] != NULL) {
                res[i] = buscaChar(j);
		buscaPadrao(arv->prox[j],padrao,res,i+1,saida);
	    }
    }
    else if ((pos != -1) && (arv->prox[pos] != NULL)) {
    	res[i] = buscaChar(pos);
        buscaPadrao(arv->prox[pos],padrao,res,i+1,saida);
    }
    else if (pos == -1) {
        printf("Erro: Caracter invalido\n");
        return;
    }

}

//Busca um titulo baseado num prefixo
void buscaPrefixo(node* arv, char *prefix, char *res, unsigned long i, FILE* saida) {

	if ((arv == NULL) || (arv->prox[MAX-1] != NULL)) {
		fputs(res,saida);
        fputs("\n",saida);
		return;
	}

    if (i < strlen(prefix)) {
        int pos = buscaPos(prefix[i]);
        if ((pos != -1) && (arv->prox[pos] != NULL)) {
            res[i] = buscaChar(pos);
            buscaPrefixo(arv->prox[pos],prefix,res,i+1,saida);
        }
    }
    else
        for (int j = 0; j < MAX; j++)
            if (arv->prox[j] != NULL) {
                res[i] = buscaChar(j);
                buscaPrefixo(arv->prox[j],prefix,res,i+1,saida);
            }
}

//Busca o maior prefixo da string fornecida
void buscaMaisLongo(node* arv, char *titulo, char *res, maisLongo *nome, unsigned long i) {

	if ((arv == NULL) || (arv->prox[MAX-1] != NULL)) {
        if ((strlen(res) > nome->tam) && (strlen(res) < strlen(titulo))){
            nome->tam = strlen(res);
            if (nome->word != NULL)
                free(nome->word);
            nome->word = strdup(res);
        }
        if ((i == strlen(titulo) - 1) || (arv == NULL))
            return;
    }
    
    int pos = buscaPos(titulo[i]);
    if ((pos != -1) && (arv->prox[pos] != NULL)) {
         res[i] = buscaChar(pos);
         buscaMaisLongo(arv->prox[pos],titulo,res,nome,i+1);
    }

}
