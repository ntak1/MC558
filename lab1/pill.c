/* Nome: Naomi Takemoto     RA: 184849
 *
 *  MC558 - Segundo Semestre 2019
 *
 * Laboratorio 1: A pilula fantastica
 *  Entrada:
 *      <m> <n>: inteiros, a quantidade de moleculas e a quantidade de
 *              ligacoes
 *      <i> <j>: inteiros, uma ligacao entre a molecula i a molecula j
 *  Saida:
 *      <dotutama> ou <doturacu ou docurama>: strings, se a pilula
 *              contem veneno ou se precisa ser testada.
 *  Descricao da Solucao:
 *      Seja G = (V, E), onde V represeta o conjunto de moleculas e E o
 *      conjunto de ligacoes. Uma molecula eh com certeza veneno se
 *      houver um ciclo de tamanho impar.
 *      Para verificar a existencia de cliclos impares, a DFS foi modificada
 *      pra que a cada vez que encontrar uma backedge (u,v) (a busca
 *      encontrou um ciclo), seja verificada se pai(u) se encontra na lista de 
 *      adjacencias de v.
 * */

#include<stdio.h>
#include<stdlib.h>
#define N_NODES m
#define N_ADJ n
#define WHITE 0
#define GRAY 2
#define BLACK 3
#define INITIAL_COLOR -1

/* ESTRUTURAS DE DADOS*/
typedef struct Node Node;
typedef struct Adj Adj;

struct Node{
    int value;
    struct Node * predecessor;
    char color;
    Adj *adj_list;
};

struct Adj{
    Node *node;
    char color;
    struct Adj *next;
};

typedef struct Graph{
    Node* nodes;
    int n_nodes;
} Graph;

typedef struct stack{
    int max_size;
    int top;
    int end;
    Node *stack_nodes;
} stack;


void create_adj(Graph* graph,int i, int j){
    Adj *i_adj_list = NULL, *j_adj_list = NULL;

    /* Insere j na lista de adj de i*/    
    i_adj_list = (Adj*)malloc(sizeof(Adj));
    i_adj_list->next = NULL;
    i_adj_list->color = INITIAL_COLOR;
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
    j_adj_list->color = INITIAL_COLOR;
    j_adj_list->node = &(graph->nodes[i]);
    if(graph->nodes[j].adj_list == NULL){
        graph->nodes[j].adj_list = j_adj_list;
    }
    else{
        j_adj_list->next = graph->nodes[i].adj_list;
        graph->nodes[j].adj_list = j_adj_list;
    }
    return;
}

void print_graph(Graph *graph){
    int i;
    Adj *temp = NULL;
    printf("GRAPH\n");
    printf("Number of Nodes: %d\n", graph->n_nodes);
    for (i = 0; i < graph->n_nodes; i++){
        printf("Node: %d\n\tAdjList: ",i+1);
        temp = graph->nodes[i].adj_list;
        while(temp != NULL){
            printf("%d ", temp->node->value);
            temp = temp->next;
        }
        printf("\n");
    }
}

int create_stack(stack *s){
    s->max_size = stack_MAX_SIZE;
    s->top = q->end = 0;
    s->stack_nodes = (Node*)malloc(sizeof(Node)*STACK_MAX_SIZE);
    if(s->stack_nodes == NULL){ret = 1;}
    else{ret = 0;}
    return ret;
}

void push(stack *s, Node * node){
    s->stack_nodes[q->top] = node;
    s->end = (q->top) + 1;
    if (s->top <= q.max_size-1){
        (s->max_size) *= 2;
        s->stack_nodes = (Node*)realloc(sizeof(Node)*(s->max_size);
    }
}

int dfs_visit(Graph *graph, Node *node){
    int ret;
    stack q;
    q = create_stack(&q);

    if(q.stack_nodes != NULL){free(q);}

    return ret;
}

int dfs(Graph *graph){
    int i;
    int ret;
    for (i = 0; i < graph->n_nodes; i++){
        if(graph->nodes[i].color == WHITE){
            ret =dfs_visit(graph, &(graph->nodes[i]));
        }
    }
    return ret;
}


int main(void){
    int m=0,n=0;
    int i=0,j=0;
    int n_adj=0;
    Graph graph;
    Node *node;

    /* Recebe a entrada do programa*/        
    scanf("%d %d", &m, &n);
    printf("m = %d, n = %d\n", m, n);

    /* Inicializa o grafo*/
    graph.nodes = (Node*)malloc(sizeof(Node)*N_NODES);
    if(graph.nodes == NULL){
        printf("Error, malloc failed!\n");
        return 1;
    }
    graph.n_nodes = N_NODES;
    for(i = 0; i < N_NODES; i++){
        graph.nodes[i].value = i+1;
        graph.nodes[i].predecessor = NULL;
        graph.nodes[i].adj_list = NULL;
        graph.nodes[i].color = WHITE;
    }

    /* Cria adjacencias*/
    while(n_adj < N_ADJ){
        scanf("%d %d", &i, &j);
        printf("i = %d, j = %d\n", i, j);
        create_adj(&graph, i-1, j-1);
        n_adj++;
    }

    print_graph(&graph);

    /* DFS*/
    
    /* Free nas variaveis*/
    return 0;
}
