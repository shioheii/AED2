/***********************************************
 * UNIFESP - Universidade Federal de São Paulo
 * Nome: Bruno Shiohei Kinoshita do Nascimento
 * RA: 176475
************************************************/

#include <stdio.h>
#include <stdlib.h>

const int MAX_VERTICES = 100;
const int INF_VALUE = 1000000;

// Estrutura da aresta
typedef struct Edge {
    int destination;
    int weight;
    struct Edge* next;
} Edge;

// Estrutura do grafo
typedef struct Graph {
    Edge** adjacencyList;
    int vertexCount;
} Graph;

// Cria um grafo
Graph* create_graph(int vertexCount) {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    graph->vertexCount = vertexCount;
    graph->adjacencyList = (Edge**)malloc(vertexCount * sizeof(Edge*));
    for (int i = 0; i < vertexCount; i++) {
        graph->adjacencyList[i] = NULL;
    }
    return graph;
}

// Insere uma aresta ordenadamente na lista de adjacência
void insert_edge(Graph* graph, int source, int destination, int weight) {
    Edge* newEdge = (Edge*)malloc(sizeof(Edge));
    newEdge->destination = destination;
    newEdge->weight = weight;
    newEdge->next = NULL;

    Edge** current = &graph->adjacencyList[source];
    while (*current != NULL && (*current)->destination < destination) {
        current = &(*current)->next;
    }

    newEdge->next = *current;
    *current = newEdge;
}

// Inicializa os vetores de distância e predecessor
void initialize_single_source(int* distance, int* predecessor, int vertexCount, int start) {
    for (int i = 0; i < vertexCount; i++) {
        distance[i] = INF_VALUE;
        predecessor[i] = -1;
    }
    distance[start] = 0;
}

// Bellman-Ford com restrição de peso
void bellman_ford(Graph* graph, int start, int restriction, int* distance, int* predecessor) {
    initialize_single_source(distance, predecessor, graph->vertexCount, start);

    for (int i = 1; i < graph->vertexCount; i++) {
        for (int u = 0; u < graph->vertexCount; u++) {
            Edge* edge = graph->adjacencyList[u];
            while (edge != NULL) {
                int v = edge->destination;
                int weight = edge->weight;
                if (weight <= restriction && distance[u] != INF_VALUE && distance[u] + weight < distance[v]) {
                    distance[v] = distance[u] + weight;
                    predecessor[v] = u;
                }
                edge = edge->next;
            }
        }
    }
}

// Imprime os menores custos do vértice inicial até os demais
void print_distances(int* distance, int vertexCount) {
    for (int i = 0; i < vertexCount; i++) {
        if (distance[i] == INF_VALUE) {
            printf("INF");
        } else {
            printf("%d", distance[i]);
        }
        if (i != vertexCount - 1) {
            printf(" ");
        }
    }
    printf("\n");
}

// Imprime o caminho do vértice inicial até o destino, se existir
void print_path(int* predecessor, int* distance, int start, int end, int maxVertices) {
    if (distance[end] == INF_VALUE) {
        printf("Destino nao alcancado\n");
        return;
    }

    int* path = (int*)malloc(maxVertices * sizeof(int));
    int size = 0;
    int current = end;
    while (current != -1) {
        path[size++] = current;
        current = predecessor[current];
    }

    for (int i = size - 1; i >= 0; i--) {
        printf("%d", path[i]);
        if (i != 0) {
            printf(" ");
        }
    }
    printf("\n");
    free(path);
}

// Libera memória da lista de adjacência
void free_graph(Graph* graph) {
    for (int i = 0; i < graph->vertexCount; i++) {
        Edge* current = graph->adjacencyList[i];
        while (current != NULL) {
            Edge* temp = current;
            current = current->next;
            free(temp);
        }
    }
    free(graph->adjacencyList);
    free(graph);
}

int main() {
    int startVertex, restriction, vertexCount, edgeCount;
    scanf("%d", &startVertex);
    scanf("%d", &restriction);
    scanf("%d %d", &vertexCount, &edgeCount);

    Graph* graph = create_graph(vertexCount);

    for (int i = 0; i < edgeCount; i++) {
        int source, destination, weight;
        scanf("%d %d %d", &source, &destination, &weight);
        insert_edge(graph, source, destination, weight);
    }

    int targetVertex;
    scanf("%d", &targetVertex);

    int* distance = (int*)malloc(vertexCount * sizeof(int));
    int* predecessor = (int*)malloc(vertexCount * sizeof(int));

    bellman_ford(graph, startVertex, restriction, distance, predecessor);
    print_distances(distance, vertexCount);
    print_path(predecessor, distance, startVertex, targetVertex, vertexCount);

    free(distance);
    free(predecessor);
    free_graph(graph);

    return 0;
}
