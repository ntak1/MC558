/* Nome: Naomi Takemoto     RA: 184849
 * S02-2019
 * LABORATORIO 3 - MC558
 * Descricao da solucao:
 *   Sistemas solares foram modelados como vertices e buracos de minhoca
 *   como arestas. Eh possivel voltar ao comeco dos tempos se houver um
 *   ciclo negativo no grafo formado dessa forma. Para identifica se
 *   tal ciclo existe foi aplicado o algoritmo de Bellman-Ford
 */

#include<stdio.h>
#include<stdlib.h>

#define N_NODES n
#define N_ADJ m

#define HEAP_SIZE 100
#define PARENT(i) i/2

#define INF 10000
#define TH 1000
#define EARTH 0
#define TRUE 1
#define FALSE 0

/*-----------------------------------------------------------------------------
 * DEFINICAO DAS ESTRUTURAS DE DADOS
 *----------------------------------------------------------------------------- */
typedef struct Node Node;
typedef struct Adj Adj;


struct Node{
    int value;
    struct Node * predecessor;
    int predecessor_weight;
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





/*-----------------------------------------------------------------------------
 * PRAGMAS
 *----------------------------------------------------------------------------- */

void create_adj(Graph* graph,int i, int j,int weight);
void print_graph(Graph *graph);
int bellman_ford(Graph *g, int source);


/*-----------------------------------------------------------------------------
 * LOGICA PRINCIPAL: LEITURA E ESCRITA DE DADOS
 *----------------------------------------------------------------------------- */

int main(void){
    int m=0,n=0;                /* numero de arestas e vertices */
    int i=0,j=0;                /* aresta  i se liga na j*/
    Graph graph;                /* grafo*/
    int n_adj=0;                /* variavel auxiliar*/
    Adj *adj_list = NULL;       /* variavel auxiliar*/
    Adj *adj_list_aux = NULL;   /* variavel auxiliar*/

    int weight;

    /* Le o numero de areas e o numero de arestas*/        
    scanf("%d %d", &n, &m);

    /* Inicializa o grafo*/
    graph.nodes = (Node*)malloc(sizeof(Node)*N_NODES);
    if(graph.nodes == NULL){return 1;}
    graph.n_nodes = N_NODES;
    for(i = 0; i < N_NODES; i++){
        graph.nodes[i].value = i;                  /* rotulo do veritice*/
        graph.nodes[i].adj_list = NULL;            /* listas de adj de cada no*/
        graph.nodes[i].key = INF;                  /* estimativa de distancia*/
    }

    /* Cria adjacencias*/
    while(n_adj < N_ADJ){
        scanf("%d %d %d", &i, &j, &weight);
        create_adj(&graph, i, j, weight);
        n_adj++;
    }

    /* Verifica a existencia de ciclos negativos*/
    if(!bellman_ford(&graph,EARTH)){
        printf("Possivel\n");
    }else{
        printf("Impossivel\n");
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
    return 0;
}

/*-----------------------------------------------------------------------------
 * FUNCOES
 *----------------------------------------------------------------------------- */



/* Aplica o algoritmo de Bellman-Ford dado um grafo g um vertice fonte source
 * Arguemntos:
 *      g: ponteiro para uma estrutura do tipo grafo
 *      source: o vertice fonte
 * Retorno:
 *      Um inteiro, 0 se houver um ciclo negativo 1 caso contrario
 */
int bellman_ford(Graph *g, int source){
    int i,k;
    Adj *edge=NULL;

    g->nodes[source].key = 0;

    /* Faz relax |V|-1 vezes*/
    for(k = 0; k < (g->n_nodes)-1; k++){
        for(i = 0; i< (g->n_nodes); i++){
            edge = g->nodes[i].adj_list;
            while(edge != NULL){
                if((g->nodes[i].key != INF)
                    && (edge->node->key > g->nodes[i].key + edge->weight)){ 
                    edge->node->key =  g->nodes[i].key + edge->weight;
                }
                edge = edge->next;
            }
        }
    }

    /* Verifica se ha ciclo negativo*/
    for(i = 0; i< (g->n_nodes); i++){
        edge = g->nodes[i].adj_list;
        while(edge != NULL){
            if( ((g->nodes[i].key != INF)
                &&(edge->node->key) > g->nodes[i].key + edge->weight)){ 
                return FALSE;
            }
            edge = edge->next;
        }
    }
    return TRUE;
}



/* Cria uma aresta orientada entre dois vertices
 * Argumentos:
 *      graph: um apontador para  uma struct do tipo Graph que contem o grafo
 *      i,j: par de vertices que sera ligado por uma aresta
 * Retorno:
 *      void
*/
void create_adj(Graph* graph,int i, int j, int weight){
    Adj *i_adj_list = NULL;

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
            printf("(%d, key %d, edge_weight %d) ", temp->node->value,
                    temp->node->key, temp->weight);
            temp = temp->next;
        }
        printf("\n");
    }
}

