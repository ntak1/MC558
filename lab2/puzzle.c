/* Nome: Naomi Takemoto     RA: 184849
 *
 *  MC558 - Segundo Semestre 2019
 *
 * Laboratorio 2: Puzzle
 *  Entrada:
 *      <m> <n>: inteiros, a quantidade de moleculas e a quantidade de
 *              ligacoes
 *      <i> <j>: inteiros, uma ligacao entre a molecula i a molecula j
 *  Saida:
 *  Descricao da Solucao:
 * */

#include<stdio.h>
#include<stdlib.h>

#define N_NODES n
#define N_ADJ n
#define MAX_WEIGHT 1000000

#define HEAP_SIZE 100
#define LEFT(i) i/2
#define RIGHT(i) i/2+1

#define WHITE 0
#define BLACK 1

/*-----------------------------------------------------------------------------
 * DEFINICAO DAS ESTRUTURAS DE DADOS
 *----------------------------------------------------------------------------- */
typedef struct Node Node;
typedef struct Adj Adj;


struct Node{
    int value;
    struct Node * predecessor;
    char color;
    int key;
    Adj *adj_list;
};

struct Adj{
    Node *node;
    int weight;
    struct Adj *next;
};

typedef struct Graph{
    Node* nodes;
    int n_nodes;
} Graph;

typedef struct Heap{
    Node ** heap;
    int size;
} Heap;


/*Rotina que conserta um heap*/
void heapify(Heap *q, int parent) {
    Node ** heap = q->heap;
    Node *temp;
    int descendant = 2*parent;
    while (descendant <= tam_heap) {
        if (descendant < tam_heap && heap[descendant]->key > heap[descendant+1]->key)
            descendant++;
        if (heap[descendant/2]->key <= heap[descendant]->key) break;
        temp = heap[descendant/2];
        heap[descendant/2] = heap[descendant];
        heap[descendant] = temp;
        descendant *= 2;/*vai para a proxima "geracao"*/
    }
}


/*transforma um vetor que nao eh um heap em um heap*/
void build_heap (Heap *q, int tam_heap) {
    int k;
    int f;
    Node **heap = q->heap;
    Node *temp;
    for (k = 1; k < tam_heap -1; ++k) {                   
        /* v[1..k] Ã© um heap*/
        f = k+1;
        /*enquanto a key do pai eh maior que o filho troca
        (eh equivalente a uma chamada de fix up)*/
        while (f > 1 && heap[f/2]->key > heap[f]->key) {  
            temp = heap[f/2];
            heap[f/2] = heap[f];
            heap[f] = temp;     
            f /= 2;                        
        }
    }
}


/* Extrai o menor valor de um heap aplicando heapfy para manter a propriedade de
 * heap
 * */
Node * extract_min(Heap *q){
    Node *min = heap[1];
    heap[1] = A[size_heap];
    q->size = (q->size) -1;
    heapify(q,1);
    return min;
}

/* Encontra o caminho minimo de source para todos os outros vertices
 * a rotina retorna ao encotrar o caminho minimo de source ate dest
 * */
void mst_prim(Graph *graph, Node *source, Node* dest, Heap *q){
    Node **heap = q->heap;
    Node *u = NULL, v* = NULL;
    Adj *edge;
    int i = 0;
    /* Initialize the keys, parents and heap*/
    for(i = 0; i < graph.n_nodes; i++){
        graph[i].key = 0;
        graph[i].color = WHITE;
        graph[i].predecessor = NULL;
        heap[i + 1] = &(graph[i]);
    } 
    source.key = 0;
    
    while(q->size > 0){
        u = extract_min(q);
        u->color = BLACK;
        edge = u->adj_list;
        while(edge != NULL){
            v = edge->node;
            /* Verifica se v esta em q e se o peso da aresta eh menor que key*/
            if( v->color == WHITE && edge->weight < v->key){
                v->predecessor = u;
                v->key = edge->weight;
            }
            edge = edge->next;
        }
    }
}


/*-----------------------------------------------------------------------------
 * PRAGMAS
 *----------------------------------------------------------------------------- */

void create_adj(Graph* graph,int i, int j);
void print_graph(Graph *graph);


/*-----------------------------------------------------------------------------
 * LOGICA PRINCIPAL: LEITURA E ESCRITA DE DADOS
 *----------------------------------------------------------------------------- */

int main(void){
    int m=0,n=0;                /* numero de moleculas ligacoes */
    int i=0,j=0;                /* ligacao: molecula i se liga na j*/
    Graph graph;                /* estrutura que contera a molecula e ligacoes*/
    int n_adj=0;                /* variavel auxiliar*/
    Adj *adj_list = NULL;       /* variavel auxiliar*/
    Adj *adj_list_aux = NULL;   /* variavel auxiliar*/

    Node **heap;
    int tam_heap;
    
    /* Le o numero de areas e o numero de ligacoes*/        
    scanf("%d %d", &n, &m);

    /* Inicializa o grafo*/
    graph.nodes = (Node*)malloc(sizeof(Node)*N_NODES);
    if(graph.nodes == NULL){return 1;}
    graph.n_nodes = N_NODES;
    for(i = 0; i < N_NODES; i++){
        graph.nodes[i].value = i+1;                /* rotulo do veritice*/
        graph.nodes[i].predecessor = NULL;         /* predecessor na arv da DFS*/
        graph.nodes[i].adj_list = NULL;            /* listas de adj de cada no*/
    }

    /* Cria adjacencias*/
    while(n_adj < N_ADJ){
        scanf("%d %d", &i, &j);
        create_adj(&graph, i-1, j-1);
        n_adj++;
    }



    /*Cria fila de prioridades*/
    heap = (Node**)malloc((N_NODES + 1)*sizeof(Node*));

    /* Desaloca memoria dinamica do grafo*/
    if(graph.nodes != NULL){
        for(i = 0; i < N_NODES; i ++){
                /* Desaloca lista de adjacencias*/
                adj_list = graph.nodes[i].adj_list;
                while(adj_list != NULL){
                    adj_list_aux = adj_list;
                    adj_list = adj_list->next;
                    free(adj_list_aux);
                }
        }
        /* Desaloca vetor de nos*/
        free(graph.nodes);
    }

    return 0;
}

/*-----------------------------------------------------------------------------
 * FUNCOES
 *----------------------------------------------------------------------------- */

/* Cria uma aresta nao orientada entre dois vertices
 * Argumentos:
 *      graph: um apontador para  uma struct do tipo Graph que contem o grafo
 *      i,j: par de vertices que sera ligado por uma aresta
 * Retorno:
 *      void
*/
void create_adj(Graph* graph,int i, int j){
    Adj *i_adj_list = NULL, *j_adj_list = NULL;

    /* Insere j na lista de adj de i*/    
    i_adj_list = (Adj*)malloc(sizeof(Adj));
    i_adj_list->next = NULL;
    i_adj_list->weight = 0;
    i_adj_list->node = &(graph->nodes[j]);
    if(graph->nodes[i].adj_list == NULL){
        graph->nodes[i].adj_list = i_adj_list;
    }
    else{
        i_adj_list->next = graph->nodes[i].adj_list;
        graph->nodes[i].adj_list = i_adj_list;
    }

    /* Insere j na lista de adj de i*/    
    j_adj_list = (Adj*)malloc(sizeof(Adj));
    j_adj_list->next = NULL;
    j_adj_list->weight = 0;
    j_adj_list->node = &(graph->nodes[i]);
    if(graph->nodes[j].adj_list == NULL){
        graph->nodes[j].adj_list = j_adj_list;
    }
    else{
        j_adj_list->next = graph->nodes[j].adj_list;
        graph->nodes[j].adj_list = j_adj_list;
    }
    return;
}




/* Imprime os nos de um grafo com seus nos vizinhos, cor da DFS, cor
 * para encontrar ciclos impares.
 * Argumentos:
 *      graph: um apontador para  uma struct do tipo Graph que contem o grafo
 * Retorno:
 *      void
 */
void print_graph(Graph *graph){
    int i;
    Adj *temp = NULL;
    printf("GRAPH\n");
    printf("Number of Nodes: %d\n", graph->n_nodes);
    for (i = 0; i < graph->n_nodes; i++){
        printf("Node: %d\n\tAdjList: ",i+1);
        temp = graph->nodes[i].adj_list;
        while(temp != NULL){
            printf("(%d, color %d, key %d) ", temp->node->value,
                   temp->node->color, temp->node->key);
            temp = temp->next;
        }
        printf("\n");
    }
}

