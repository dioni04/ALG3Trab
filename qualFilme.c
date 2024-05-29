#include "trie.h"

int main(int argc, char **argv) {

    node *arv;

    //Inicializa a raiz da arvore
    if ((arv = criaNode()) == NULL) {
        printf("Erro na criacao da arvore\n");
        return 1;
    }

    //Passa os dados do arquivo de entrada p/ a arvore
    if ((arv = processaArq(argv[1],arv)) == NULL) {
        printf("Erro no processamento do arquivo de entrada\n");
        return 1;
    }

    FILE* consulta;
    //Abre o arquivo de consulta
    if ((consulta = fopen(argv[2],"r")) == NULL) {
        printf("Erro ao abrir o arquivo de consulta\n");
        return 1;
    }
    
    FILE* saida;
    //Abre o arquivo de saida
    if ((saida = fopen(argv[3],"w+")) == NULL) {
        printf("Erro ao abrir o arquivo de saida\n");
        return 1;
    }
    
    char linha[TAM_LINHA], *buffer, *padrao, *op;

    //Executa esse loop p/ todas as linhas do arquivo de consulta
    while (fgets(linha,TAM_LINHA,consulta) != NULL) {
        fputs(linha,saida);
        buffer = linha;
        buffer[strcspn(buffer, "\n")] = 0;  //Substitui o \n por \0 no fim da linha
        op = strsep(&buffer," ");           //Op eh a operacao a ser realizada, no caso, p, l ou c
        padrao = strsep(&buffer, "\n");
        busca(arv,padrao,op, saida);             
    }

    fclose(consulta);
    fclose(saida);

    freeArv(arv);

    return 0;
}
