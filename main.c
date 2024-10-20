#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <float.h>
#include <string.h>

#define MAX_ARESTAS 100
#define MAX_VERTICES 100
#define INFINITO DBL_MAX

//Limpa a tela com base no sistema operacional
void clear() {
    #ifdef _WIN32
        system("cls");  //Para Windows
    #else
        system("clear"); //Para Unix/Linux/Mac
    #endif
}

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

typedef struct QueueNode {
    Aresta aresta;
    struct QueueNode* next;
} QueueNode;

//Estrutura da fila
typedef struct {
    QueueNode* front;
    QueueNode* rear;
} Queue;

//Estrutura da pilha - Started
typedef struct StackNode {
    Aresta aresta;
    struct StackNode* next;
} StackNode;

typedef struct {
    StackNode* top;
} Stack;

void initStack(Stack* s) {
    s->top = NULL;
}

bool isStackEmpty(Stack* s) {
    return s->top == NULL;
}

void push(Stack* s, Aresta a) {
    StackNode* newNode = (StackNode*)malloc(sizeof(StackNode));
    newNode->aresta = a;
    newNode->next = s->top;
    s->top = newNode;
}

Aresta pop(Stack* s) {
    if (isStackEmpty(s)) {
        Aresta emptyAresta = {0}; //Retorna uma aresta vazia
        return emptyAresta;
    }

    StackNode* temp = s->top;
    Aresta a = temp->aresta;
    s->top = s->top->next;
    free(temp);
    return a;
}
//Estrutura da pilha - Ended


//Caminho mínimo, configurações iniciais - Started
typedef struct {
    double dist;  //Distância mínima do vértice inicial
    int anterior; //Índice do vértice anterior no caminho
} Distancia;
//Caminho mínimo, configurações iniciais - Ended

Aresta grafo[MAX_ARESTAS];
int numArestas = 0;


void ler_grafo(const char* nomeArquivo) {
    FILE *file = fopen(nomeArquivo, "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo: %s\n", nomeArquivo);
        return;
    }

    while (1) {
        Aresta a;
        int result = fscanf(file, "%d %lf %lf %lf", &a.id, &a.impedanciaPositiva, &a.impedanciaNegativa, &a.demanda);
        
        //Verifica se todas as informações foram lidas corretamente
        if (result != 4) break; 

        if (fscanf(file, "%lf %lf", &a.xInicial, &a.yInicial) != 2) {
            printf("Erro ao ler coordenadas iniciais.\n");
            break;
        }

        if (fscanf(file, "%lf %lf", &a.xFinal, &a.yFinal) != 2) {
            printf("Erro ao ler coordenadas finais.\n");
            break;
        }

        //Adiciona a aresta ao grafo
        grafo[numArestas++] = a; 

        //Lê o delimitador "FIM"
        char delimiter[4];
        fscanf(file, "%s", delimiter);
        if (strcmp(delimiter, "FIM") != 0) {
            printf("Erro: esperado 'FIM' após a aresta.\n");
            break;
        }

        //Verifica se o número máximo de arestas foi atingido
        if (numArestas >= MAX_ARESTAS) {
            printf("Número máximo de arestas atingido.\n");
            break;
        }
    }

    fclose(file);

    printf("Arestas lidas do grafo:\n");
    for (int i = 0; i < numArestas; i++) {
        printf("Id: %d, Impedancia Positiva: %.6lf, Impedancia Negativa: %.6lf, Demanda: %.6lf, "
               "Coordenadas Inicial: (%.6lf, %.6lf), Coordenadas Final: (%.6lf, %.6lf)\n",
               grafo[i].id, grafo[i].impedanciaPositiva, grafo[i].impedanciaNegativa, 
               grafo[i].demanda, grafo[i].xInicial, grafo[i].yInicial, 
               grafo[i].xFinal, grafo[i].yFinal);
    }
}


//Busca em Largura - Start
//Funções da fila
void initQueue(Queue* q) {
    q->front = NULL;
    q->rear = NULL;
}

bool isEmpty(Queue* q) {
    return q->front == NULL;
}

void enqueue(Queue* q, Aresta a) {
    QueueNode* newNode = (QueueNode*)malloc(sizeof(QueueNode));
    newNode->aresta = a;
    newNode->next = NULL;

    if (isEmpty(q)) {
        q->front = newNode;
        q->rear = newNode;
    } else {
        q->rear->next = newNode;
        q->rear = newNode;
    }
}

Aresta dequeue(Queue* q) {
    if (isEmpty(q)) {
        Aresta emptyAresta = {0}; //Retorna uma aresta vazia
        return emptyAresta;
    }

    QueueNode* temp = q->front;
    Aresta a = temp->aresta;
    q->front = q->front->next;
    if (q->front == NULL) {
        q->rear = NULL;
    }
    free(temp);
    return a;
}

//Função de busca em largura
void busca_em_largura(double xInicial, double yInicial) {
    bool visitado[MAX_ARESTAS] = {false}; //Marca se uma aresta foi visitada
    Queue fila;
    initQueue(&fila);

    //Encontra a aresta inicial
    Aresta* arestaInicial = NULL;
    for (int i = 0; i < numArestas; i++) {
        if (grafo[i].xInicial == xInicial && grafo[i].yInicial == yInicial) {
            arestaInicial = &grafo[i];
            break;
        }
    }

    if (arestaInicial == NULL) {
        printf("Aresta inicial nao encontrada com as coordenadas (%lf, %lf).\n", xInicial, yInicial);
        return;
    }

    //Enfileira a aresta inicial e marca como visitada
    enqueue(&fila, *arestaInicial);
    visitado[arestaInicial->id] = true;

    printf("Vertices visitados (BFS):\n");

    while (!isEmpty(&fila)) {
        Aresta atual = dequeue(&fila);
        printf("Coordenadas: (%.6lf, %.6lf)\n", atual.xInicial, atual.yInicial);

        //Verifica todas as arestas para adjacência
        for (int i = 0; i < numArestas; i++) {
            //Se a aresta não foi visitada e é adjacente
            if (!visitado[grafo[i].id] && (grafo[i].xInicial == atual.xFinal && grafo[i].yInicial == atual.yFinal)) {
                enqueue(&fila, grafo[i]);
                visitado[grafo[i].id] = true;
            }
        }
    }
}
//Busca em Largura - Ended

//Função de busca em profundidade - Started
void busca_em_profundidade(double xInicial, double yInicial) {
    bool visitado[MAX_ARESTAS] = {false}; //Marca se uma aresta foi visitada
    Stack pilha;
    initStack(&pilha);

    //Encontra a aresta inicial
    Aresta* arestaInicial = NULL;
    for (int i = 0; i < numArestas; i++) {
        if (grafo[i].xInicial == xInicial && grafo[i].yInicial == yInicial) {
            arestaInicial = &grafo[i];
            break;
        }
    }

    if (arestaInicial == NULL) {
        printf("Aresta inicial nao encontrada com as coordenadas (%lf, %lf).\n", xInicial, yInicial);
        return;
    }

    //Empilha a aresta inicial e marca como visitada
    push(&pilha, *arestaInicial);
    visitado[arestaInicial->id] = true;

    printf("Vertices visitados (DFS):\n");

    while (!isStackEmpty(&pilha)) {
        Aresta atual = pop(&pilha);
        printf("Coordenadas: (%.6lf, %.6lf)\n", atual.xInicial, atual.yInicial);

        //Verifica todas as arestas para adjacência
        for (int i = 0; i < numArestas; i++) {
            //Se a aresta não foi visitada e é adjacente
            if (!visitado[grafo[i].id] && (grafo[i].xInicial == atual.xFinal && grafo[i].yInicial == atual.yFinal)) {
                push(&pilha, grafo[i]);
                visitado[grafo[i].id] = true;
            }
        }
    }
}
//Função de busca em profundidade - Ended


//unções para caminho minimo - Started

//Função para encontrar o índice de um vértice no grafo
int encontrar_vertice(double x, double y) {
    for (int i = 0; i < numArestas; i++) {
        if ((grafo[i].xInicial == x && grafo[i].yInicial == y) ||
            (grafo[i].xFinal == x && grafo[i].yFinal == y)) {
            return i; //Retorna o índice de uma aresta que contém esse vértice
        }
    }
    return -1; //Vértice não encontrado
}


//Função para encontrar o índice do vértice mais próximo não visitado
int encontrar_vertice_mais_proximo(Distancia distancias[], bool visitado[]) {
    double minDist = INFINITO;
    int minIndex = -1;

    for (int i = 0; i < numArestas; i++) {
        if (!visitado[i] && distancias[i].dist < minDist) {
            minDist = distancias[i].dist;
            minIndex = i;
        }
    }

    return minIndex;
}

//Função de Dijkstra para calcular o caminho mínimo
void caminho_minimo(double xInicial, double yInicial, double xFinal, double yFinal) {
    Distancia distancias[MAX_ARESTAS];
    bool visitado[MAX_ARESTAS] = {false};

    //Inicializa todas as distâncias como infinitas e sem predecessores
    for (int i = 0; i < numArestas; i++) {
        distancias[i].dist = INFINITO;
        distancias[i].anterior = -1;
    }

    int inicial = encontrar_vertice(xInicial, yInicial);
    if (inicial == -1) {
        printf("Vertice inicial nao encontrado.\n");
        return;
    }

    //Define a distância do vértice inicial para ele mesmo como 0
    distancias[inicial].dist = 0;

    for (int i = 0; i < numArestas - 1; i++) {
        //Encontra o vértice com a menor distância não visitado
        int u = encontrar_vertice_mais_proximo(distancias, visitado);

        if (u == -1) {
            break; //Se não houver mais vértices alcançáveis
        }

        visitado[u] = true; //Marca o vértice como visitado

        //Atualiza as distâncias para os vértices adjacentes
        for (int v = 0; v < numArestas; v++) {
            if (!visitado[v] && grafo[v].xInicial == grafo[u].xFinal && grafo[v].yInicial == grafo[u].yFinal) {
                double peso = grafo[u].impedanciaPositiva; 
                if (distancias[u].dist + peso < distancias[v].dist) {
                    distancias[v].dist = distancias[u].dist + peso;
                    distancias[v].anterior = u;
                }
            }
        }
    }

    //Verifica se o vértice final foi alcançado
    int final = encontrar_vertice(xFinal, yFinal);
    if (final == -1 || distancias[final].dist == INFINITO) {
        printf("Nao ha caminho disponivel para o vertice final.\n");
        return;
    }

    //Exibe o caminho mínimo encontrado
    printf("Caminho minimo de (%.6lf, %.6lf) para (%.6lf, %.6lf):\n", xInicial, yInicial, xFinal, yFinal);
    int atual = final;
    Stack caminho;
    initStack(&caminho);

    //Armazena o caminho na pilha para exibir na ordem correta
    while (atual != -1) {
        push(&caminho, grafo[atual]);
        atual = distancias[atual].anterior;
    }

    //Exibe o caminho armazenado na pilha
    while (!isStackEmpty(&caminho)) {
        Aresta a = pop(&caminho);
        printf("Id: %d, Impedancia: %.6lf, Coordenadas: (%.6lf, %.6lf) -> (%.6lf, %.6lf)\n",
               a.id, a.impedanciaPositiva, a.xInicial, a.yInicial, a.xFinal, a.yFinal);
    }

    printf("Distancia total: %.6lf\n", distancias[final].dist);
}
//Funções para caminho minimo - Ended

int main() {
    int res = 0;

    while (res != 6) {
        clear();
        printf("\n---------------------------------------------\n");
        printf("\n\tMENU\n\n");
        printf("\t1 - Ler Grafo de um Arquivo\n");
        printf("\t2 - Busca em Largura\n");
        printf("\t3 - Busca em Profundidade\n");
        printf("\t4 - Caminho Minimo\n");
        printf("\t5 - Sobre\n");
        printf("\t6 - Sair\n");
        printf("\n\tResposta: ");
        scanf("%i", &res);

        switch (res) {
            case 1: {
                char nomeArquivo[100]; 
                printf("Digite o nome do arquivo (com .txt): ");
                scanf("%s", nomeArquivo); 
                ler_grafo(nomeArquivo); 
                printf("\nPressione Enter para continuar...\n");
                getchar(); 
                getchar(); 
                break;
            }
            case 2: {
                double xInicial, yInicial;
                printf("Digite a latitude (x) do vertice inicial: ");
                scanf("%lf", &xInicial);
                printf("Digite a longitude (y) do vertice inicial: ");
                scanf("%lf", &yInicial);
                busca_em_largura(xInicial, yInicial);
                printf("\nPressione Enter para continuar...\n");
                getchar(); 
                getchar(); 
                break;
            }
            case 3: {
                double xInicial, yInicial;
                printf("Digite a latitude (x) do vertice inicial: ");
                scanf("%lf", &xInicial);
                printf("Digite a longitude (y) do vertice inicial: ");
                scanf("%lf", &yInicial);
                busca_em_profundidade(xInicial, yInicial);
                printf("\nPressione Enter para continuar...\n");
                getchar(); 
                getchar(); 
                break;
            }
            case 4: {
                double xInicial, yInicial, xFinal, yFinal;
                printf("Digite a latitude (x) do vertice inicial: ");
                scanf("%lf", &xInicial);
                printf("Digite a longitude (y) do vertice inicial: ");
                scanf("%lf", &yInicial);
                printf("Digite a latitude (x) do vertice final: ");
                scanf("%lf", &xFinal);
                printf("Digite a longitude (y) do vertice final: ");
                scanf("%lf", &yFinal);
                caminho_minimo(xInicial, yInicial, xFinal, yFinal);
                printf("\nPressione Enter para continuar...\n");
                getchar(); 
                getchar(); 
                break;
            }
            case 5:
                printf("\n\nGrafo Alguma Coisa - Rennan e Arnald\n\n");
                break;
            case 6:
                printf("\n\tAte a proxima!\n");
                printf("\n---------------------------------------------\n");
                break;
            default:
                printf("Opcao invalida\n");
                break;
        }
    }

    return 0;
}
