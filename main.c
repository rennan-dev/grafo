#include <stdio.h>
#include <stdlib.h>

#define MAX_ARESTAS 100  // Defina um tamanho máximo para as arestas

typedef struct {
    int id;
    double impedanciaPositiva;
    double impedanciaNegativa;
    double demanda;
    double xInicial;
    double yInicial;
    double xFinal;
    double yFinal;
} Aresta;

Aresta grafo[MAX_ARESTAS];
int numArestas = 0;

// Limpa a tela com base no sistema operacional
void clear() {
    #ifdef _WIN32
        system("cls");  // Para Windows
    #else
        system("clear"); // Para Unix/Linux/Mac
    #endif
}

void ler_grafo() {
    FILE *file = fopen("grafo01.txt", "r"); 
    if (file == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }

    while (1) {
        Aresta a;
        // Lê os dados da aresta
        int result = fscanf(file, "%d %lf %lf %lf", &a.id, &a.impedanciaPositiva, &a.impedanciaNegativa, &a.demanda);
        if (result == EOF) break; // Fim do arquivo
        if (result != 4) { // Verifica se leu todos os dados esperados
            printf("Erro ao ler aresta: esperava 4 valores, mas obteve %d.\n", result);
            break;
        }

        // Lê as coordenadas
        if (fscanf(file, "%lf %lf", &a.xInicial, &a.yInicial) != 2) {
            printf("Erro ao ler coordenadas iniciais.\n");
            break;
        }
        if (fscanf(file, "%lf %lf", &a.xFinal, &a.yFinal) != 2) {
            printf("Erro ao ler coordenadas finais.\n");
            break;
        }

        grafo[numArestas++] = a; // Armazena a aresta
        fscanf(file, "%*s"); // Ignora a palavra "FIM"

        // Verifica se o número máximo de arestas foi atingido
        if (numArestas >= MAX_ARESTAS) {
            printf("Número máximo de arestas atingido.\n");
            break;
        }
    }

    fclose(file);

    // Exibe as arestas lidas
    printf("Arestas lidas do grafo:\n");
    for (int i = 0; i < numArestas; i++) {
        printf("Id: %d, Impedancia Positiva: %.6lf, Impedancia Negativa: %.6lf, Demanda: %.6lf, "
               "Coordenadas Inicial: (%.6lf, %.6lf), Coordenadas Final: (%.6lf, %.6lf)\n",
               grafo[i].id, grafo[i].impedanciaPositiva, grafo[i].impedanciaNegativa, 
               grafo[i].demanda, grafo[i].xInicial, grafo[i].yInicial, 
               grafo[i].xFinal, grafo[i].yFinal);
    }
}

int main() {
    int res=0;

    while(res!=5) {
        clear();
        printf("\n---------------------------------------------\n");
        printf("\n\tMENU\n\n");
        printf("\t1 - Ler Grafo de um Arquivo\n");
        printf("\t2 - Busca em Largura\n");
        printf("\t3 - Busca em Profundidade\n");
        printf("\t4 - Sobre\n");
        printf("\t5 - Sair\n");
        printf("\n\tResposta: ");
        scanf("%i", &res);

        switch(res) {
            case 1: 
                ler_grafo(); 
                printf("\nPressione Enter para continuar...\n");
                getchar(); // Consome o caractere de nova linha
                getchar(); // Espera o usuário pressionar Enter
                break;
            case 5:
                printf("\n\tAte a proxima!\n");
                printf("\n---------------------------------------------\n"); 
                break;
            default:
                printf("Opcao invalida"); 
                break;
        }
    }

    return 0;
}
