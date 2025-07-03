/***********************************************
 * UNIFESP - Universidade Federal de São Paulo
 * Nome: Bruno Shiohei Kinoshita do Nascimento
 * RA: 176475
************************************************/

#include <stdio.h>
#include <stdlib.h>

// Limite máximo de vértices permitido no grafo
const int VERTEX_LIMIT = 100;

// Estrutura para representar um nó da lista de adjacência
typedef struct node {
    int vertex;
    struct node *next;
} Node;

// Estrutura que representa a lista de adjacência de um vértice
typedef struct {
    Node *head;
} AdjList;

// Estrutura principal do grafo com um vetor de listas de adjacência
typedef struct {
    int vertexCount;
    AdjList *array;
} Graph;

// Estrutura de dados fila, usada na busca em largura
typedef struct {
    int *items;
    int front;
    int rear;
    int size;
} Queue;

// Cria e inicializa um grafo com a quantidade informada de vértices
Graph *create_graph(int vertexCount) {
    Graph *graph = (Graph *)malloc(sizeof(Graph));
    graph->vertexCount = vertexCount;
    graph->array = (AdjList *)malloc(vertexCount * sizeof(AdjList));
    for (int i = 0; i < vertexCount; i++) {
        graph->array[i].head = NULL;
    }
    return graph;
}

// Cria um novo nó de lista de adjacência
Node *create_node(int vertex) {
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->vertex = vertex;
    newNode->next = NULL;
    return newNode;
}

// Insere um novo vértice na lista de forma ordenada (ordem crescente)
void insert_node_in_order(Node **head, int vertex) {
    Node *newNode = create_node(vertex);
    if (*head == NULL || (*head)->vertex > vertex) {
        newNode->next = *head;
        *head = newNode;
    } else {
        Node *current = *head;
        while (current->next != NULL && current->next->vertex < vertex) {
            current = current->next;
        }
        newNode->next = current->next;
        current->next = newNode;
    }
}

// Adiciona uma aresta direcionada ao grafo de src para dest
void add_directed_edge(Graph *graph, int src, int dest) {
    insert_node_in_order(&graph->array[src].head, dest);
}

// Cria e inicializa uma fila com capacidade definida
Queue *create_queue(int capacity) {
    Queue *queue = (Queue *)malloc(sizeof(Queue));
    queue->items = (int *)malloc(capacity * sizeof(int));
    queue->front = 0;
    queue->rear = -1;
    queue->size = 0;
    return queue;
}

// Insere um elemento na fila
void enqueue_item(Queue *queue, int value) {
    queue->items[++queue->rear] = value;
    queue->size++;
}

// Remove e retorna o primeiro elemento da fila
int dequeue_item(Queue *queue) {
    queue->size--;
    return queue->items[queue->front++];
}

// Verifica se a fila está vazia
int is_queue_empty(Queue *queue) {
    return queue->size == 0;
}

// Executa a busca em largura (BFS) a partir de um vértice inicial
void breadth_first_search(Graph *graph, int startVertex) {
    int visited[VERTEX_LIMIT];
    for (int i = 0; i < VERTEX_LIMIT; i++) {
        visited[i] = 0;
    }

    Queue *queue = create_queue(VERTEX_LIMIT);
    enqueue_item(queue, startVertex);
    visited[startVertex] = 1;

    while (!is_queue_empty(queue)) {
        int currentVertex = dequeue_item(queue);
        printf("%d ", currentVertex);

        Node *temp = graph->array[currentVertex].head;
        while (temp != NULL) {
            int adjVertex = temp->vertex;
            if (!visited[adjVertex]) {
                visited[adjVertex] = 1;
                enqueue_item(queue, adjVertex);
            }
            temp = temp->next;
        }
    }

    printf("\n");
    free(queue->items);
    free(queue);
}

// Função auxiliar recursiva para a busca em profundidade (DFS)
void depth_first_search_recursive(Graph *graph, int vertex, int *visited) {
    visited[vertex] = 1;
    printf("%d ", vertex);

    Node *temp = graph->array[vertex].head;
    while (temp != NULL) {
        int adjVertex = temp->vertex;
        if (!visited[adjVertex]) {
            depth_first_search_recursive(graph, adjVertex, visited);
        }
        temp = temp->next;
    }
}

// Executa a busca em profundidade (DFS) a partir de um vértice inicial
void depth_first_search(Graph *graph, int startVertex) {
    int visited[VERTEX_LIMIT];
    for (int i = 0; i < VERTEX_LIMIT; i++) {
        visited[i] = 0;
    }

    depth_first_search_recursive(graph, startVertex, visited);
    printf("\n");
}

// Libera toda a memória alocada dinamicamente pelo grafo
void free_graph_memory(Graph *graph) {
    for (int i = 0; i < VERTEX_LIMIT; i++) {
        Node *current = graph->array[i].head;
        while (current != NULL) {
            Node *next = current->next;
            free(current);
            current = next;
        }
    }
    free(graph->array);
    free(graph);
}

int main() {
    int startVertex, nVertices, nEdges, src, dest;
    scanf("%d", &startVertex);
    scanf("%d %d", &nVertices, &nEdges);

    Graph *graph = create_graph(VERTEX_LIMIT);

    for (int i = 0; i < nEdges; i++) {
        scanf("%d %d", &src, &dest);
        add_directed_edge(graph, src, dest);
    }

    breadth_first_search(graph, startVertex);
    depth_first_search(graph, startVertex);

    free_graph_memory(graph);

    return 0;
}
