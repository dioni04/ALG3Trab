#include "trie.h"

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


void freeArv(node* arv) {
    
    if (arv == NULL)
        return;

    for(int i = 0; i < MAX; i++) 
        if (arv->prox[i] != NULL)
            freeArv(arv->prox[i]);
    
    free(arv);
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

    aux->prox[0] = criaNode();  //O ponteiro que representa '\0' sera diferente de nulo ao fim de cada palavra

    return raiz;
}

//Limpa resto da string
void cleanStr(char* s, int i){
    for(i = i; i < MAX; i++)
        s[i] = 0;
    return;
}

//Retorna a posicao correta do char no vetor
int buscaPos(char c) {

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

//Retorna um caracter baseado na posicao do vetor, oposto da funcao buscaPos
char buscaChar(int pos) {

    if (pos == 38)
        return '?';
    // Letras lowercase
    if(pos >= 12){
        pos -= 12;
        return 'a' + pos;
    }
    // Numeros 0-9
    if(pos >= 2){
        pos -= 2;
        return '0' + pos;
    }

    if(pos == 1)
        return ' ';

    if(pos == 0)
        return '\0';

    return '?'; //Desconhecido
        
}

//Essa funcao chama as funcoes adequadas baseado no op
void busca(node* arv, char *string, char *op, FILE* saida) {

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

//Funcao auxiliar para coringa *
void buscaPadraoAux(node* arv, char* padrao, char* res, int i, unsigned long n, int proxPos, FILE* saida){

    if(arv->prox[proxPos] != NULL){ //Se for nao NULL o prox volta para funcao que chamou
        cleanStr(res, i);
        res[i] = buscaChar(proxPos);
        buscaPadrao(arv, padrao, res, n, saida);
    }

    for(int j = 0; j < MAX; j++){ //Percorre vetor e vai em todas sub arvores
        if (arv->prox[j] != NULL) {
            res[i] = buscaChar(j);
            buscaPadraoAux(arv->prox[j], padrao, res, i+1, n,proxPos, saida);
        }
    }
    return;
}

//No momento ela so busca com o wildcard '.', mas dps eh so adpatar pra funcionar pra '*' tbm
void buscaPadrao(node* arv, char *padrao, char *res, unsigned long i, FILE* saida) {  
    
    if ((arv == NULL) || (i >= strlen(padrao))) {
        if (arv->prox[0] != NULL) {
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
    else if (padrao[i] == '*'){
        int proxPos;
        int aux = 1;

        if(padrao[i+1] != '.' && padrao[i+1] != '*')
            proxPos = buscaPos(padrao[i+1]);
        else{ //Acha proximo caracter valido no padrao
            aux = 2;

            while(padrao[i + aux] == '.' || padrao[i + aux] == '*')
                aux++;
            proxPos = buscaPos(padrao[i+aux]); //O . nao faz diferenca
        }
        buscaPadraoAux(arv, padrao, res, i, i + aux, proxPos, saida);
        return;
    }
    else if ((pos != -1) && (arv->prox[pos] != NULL)) {
    	res[i] = buscaChar(pos);
        buscaPadrao(arv->prox[pos],padrao,res,i+1,saida);
    }
    else if (pos == -1) {
        printf("Erro: Caracter invalido\n");
        return;
    }
    return;
}

//Busca um titulo baseado num prefixo
void buscaPrefixo(node* arv, char *prefix, char *res, unsigned long i, FILE* saida) {

	if ((arv == NULL) || (arv->prox[0] != NULL)) {
        cleanStr(res, i);
		fputs(res,saida);
        fputs("\n",saida);
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
    return;
}

//Busca o maior prefixo da string fornecida
void buscaMaisLongo(node* arv, char *titulo, char *res, maisLongo *nome, unsigned long i) {

	if ((arv == NULL) || (arv->prox[0] != NULL)) {
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
