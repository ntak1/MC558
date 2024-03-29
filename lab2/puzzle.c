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
#define N_ADJ m
#define MAX_WEIGHT 1000000

#define HEAP_SIZE 100
#define PARENT(i) i/2

#define WHITE 0
#define BLACK 1
#define INF 1000001

/*-----------------------------------------------------------------------------
 * DEFINICAO DAS ESTRUTURAS DE DADOS
 *----------------------------------------------------------------------------- */
typedef struct Node Node;
typedef struct Adj Adj;


struct Node{
    int value;
    struct Node * predecessor;
    int predecessor_weight;
    char color;
    int key;
    int heap_pos;
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
    while (descendant <= q->size) {
        if (descendant <= q->size && heap[descendant]->key > heap[descendant+1]->key)
            descendant++;
        if (heap[descendant/2]->key <= heap[descendant]->key) break;
        temp = heap[descendant/2];
        heap[descendant/2] = heap[descendant];
        heap[descendant/2]->heap_pos = descendant/2;
        heap[descendant] = temp;
        heap[descendant]->heap_pos = descendant;
        descendant *= 2;/*vai para a proxima "geracao"*/
    }
}


/*transforma um vetor que nao eh um heap em um heap*/
void build_heap (Heap *q) {
    int k;
    int f;
    Node **heap = q->heap;
    Node *temp;
    for (k = 1; k < (q->size); k++) {                   
        /* v[1..k] Ã© um heap*/
        f = k+1;
        /*enquanto a key do pai eh maior que o filho troca
        (eh equivalente a uma chamada de fix up)*/
        while (f > 1 && heap[f/2]->key > heap[f]->key) {  
            temp = heap[f/2];
            heap[f/2] = heap[f];
            heap[f/2]->heap_pos = f/2;
            heap[f] = temp;     
            heap[f]->heap_pos = f;     
            f /= 2;                        
        }
    }
}


/* Extrai o menor valor de um heap aplicando heapfy para manter a propriedade de
 * heap
 * */
Node * extract_min(Heap *q){
    Node **heap = q->heap;
    Node *min = heap[1];
    heap[1] = heap[q->size];
    heap[1]->heap_pos = 1;
    q->size = (q->size) -1;
    heapify(q,1);
    return min;
}

/* Diminui o valor de key de um dos nós do heap e conserta o heap
 * Argumentos:
 *  q: ponteiro para o uma variavel do tipo Heap
 *  i: o indice do elemento no heap
 * */
void decrease_key(Heap *q, Node * node, int new_value){
    Node **heap = q->heap;
    node->key = new_value;
    Node *temp;
    int parent = 0;
    int i;
    i = node->heap_pos;

    /*printf("DECREASE KEY %d\n", node->value);
    */
    while(i > 1  && (heap[PARENT(i)]->key) > (heap[i]->key)){
        /* troca pai e filho e atualiza a posicao do no*/
        temp = heap[i];
        parent = PARENT(i);
        heap[i] = heap[parent];      /*coloca o pai no lugar do filho*/
        heap[i]->heap_pos = i;   /*nova posicao do pai*/

        heap[parent] = temp;         /*coloca o filho no lugar do pai*/
        heap[parent]->heap_pos = parent;   /*nova posicao do filho*/

        i = PARENT(i);
    }
    
}


void print_heap(Heap*q){
    int i;
    Node **heap = q->heap;
    printf("HEAP: ");
    for(i = 1; i <= q->size; i++){
        printf("(value = %d, key = %d pos = %d)", heap[i]->value, heap[i]->key, heap[i]->heap_pos);
    }
    printf("\n");
}


/* Encontra o caminho minimo de source para todos os outros vertices
 * a rotina retorna ao encotrar o caminho minimo de source ate dest
 * */
void mst_prim(Graph *graph, int source_index, int dest_index, Heap *q){
    Node **heap = q->heap;
    Node *u = NULL, *v = NULL;
    Adj *edge;
    int i = 0, ret =0;
    Node *source = NULL, *dest=NULL;
    source = &(graph->nodes[source_index]);
    dest = &(graph->nodes[dest_index]);

    /* Initialize the keys, parents and heap*/
    for(i = 0; i <(graph->n_nodes); i++){
        graph->nodes[i].key = INF;
        graph->nodes[i].color = WHITE;
        graph->nodes[i].predecessor = NULL;
        graph->nodes[i].predecessor_weight = 0;
        graph->nodes[i].heap_pos = i+1;
        heap[i + 1] = &(graph->nodes[i]);
    } 
    /*printf("Source value %d\n", source->value);
    */
    source->key = 0;
    q->size = (graph->n_nodes);

    build_heap(q);
    
    /*print_heap(q);
    printf("\n");
    */
    /* Sanity check */

    while(q->size > 0 && !ret){
        u = extract_min(q);
        u->color = BLACK;
        edge = u->adj_list;
        /*printf("U (value = %d, key = %d)\n", u->value, u->key);
        */
        while(edge != NULL){
            v = edge->node;
            /* Verifica se v esta em q e se o peso da aresta eh menor que key*/
            if( v->color == WHITE && edge->weight < v->key){
                v->predecessor = u;
                v->predecessor_weight = edge->weight;
                /*DECREASE-KEY*/
                decrease_key(q, v, edge->weight);
                /*printf("V (value = %d, key = %d)\n", v->value, v->key);
                print_heap(q);
                printf("\n");
                */
            }

            edge = edge->next;
        }
        if (u == dest && u->predecessor != NULL){
            ret = 1;
            break;
        }
    }
}


/*-----------------------------------------------------------------------------
 * PRAGMAS
 *----------------------------------------------------------------------------- */

void create_adj(Graph* graph,int i, int j,int weight);
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

    Heap q;
    Node **heap;
    int weight;
    
    int n_pairs = 0;
    int source_index, dest_index;
    int path_weight;
    int max_edge_weight = 0;

    Node *source = NULL, *dest = NULL, *node_temp = NULL;

    /* Le o numero de areas e o numero de ligacoes*/        
    scanf("%d %d", &n, &m);

    /* Inicializa o grafo*/
    graph.nodes = (Node*)malloc(sizeof(Node)*N_NODES);
    if(graph.nodes == NULL){return 1;}
    graph.n_nodes = N_NODES;
    for(i = 0; i < N_NODES; i++){
        graph.nodes[i].value = i;                /* rotulo do veritice*/
        graph.nodes[i].predecessor = NULL;         /* predecessor na arv da DFS*/
        graph.nodes[i].predecessor_weight = 0;
        graph.nodes[i].adj_list = NULL;            /* listas de adj de cada no*/
    }

    /* Cria adjacencias*/
    while(n_adj < N_ADJ){
        scanf("%d %d %d", &i, &j, &weight);
        create_adj(&graph, i, j, weight);
        n_adj++;
    }

    /* Sanity check*/

    /* ALoca espaco para fila de prioridades*/
    heap = (Node**)malloc((N_NODES + 1)*sizeof(Node*));
    q.heap = heap;
    q.size = N_NODES;

    /*Le os pares de vertices*/
    scanf("%d", &n_pairs);
    for(i = 0; i < n_pairs; i++){
        scanf("%d %d", &source_index, &dest_index);

        /* Busca mst para cada par de vertices*/
        /*printf("(source = %d, index = %d)\n", source_index, dest_index);
        */
        mst_prim(&graph, source_index, dest_index, &q);

        /*Calcula o peso do caminho minimo a partir dos predecessores*/
        path_weight = 0;
        source = &(graph.nodes[source_index]);
        dest = &(graph.nodes[dest_index]);
        node_temp = dest;
        max_edge_weight = 0;
        while(node_temp != NULL){
            if(node_temp->predecessor_weight > max_edge_weight){
                max_edge_weight = node_temp->predecessor_weight;
            }
            path_weight = path_weight + (node_temp->predecessor_weight);
            node_temp = node_temp->predecessor;
            if (node_temp == source) break;
        }
        printf("%d\n", max_edge_weight);

    }

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
    
    if(heap != NULL){
        free(heap);
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
void create_adj(Graph* graph,int i, int j, int weight){
    Adj *i_adj_list = NULL, *j_adj_list = NULL;

    /* Insere j na lista de adj de i*/    
    i_adj_list = (Adj*)malloc(sizeof(Adj));
    i_adj_list->next = NULL;
    i_adj_list->weight = weight;
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
    j_adj_list->weight = weight;
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
        printf("Node: %d\n\tAdjList: ",i);
        temp = graph->nodes[i].adj_list;
        while(temp != NULL){
            printf("(%d, color %d, key %d, edge_weight %d) ", temp->node->value,
                   temp->node->color, temp->node->key, temp->weight);
            temp = temp->next;
        }
        printf("\n");
    }
}

