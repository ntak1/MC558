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
#define GRAY 1
#define INITIAL_COLOR -1
#define STACK_MAX_SIZE 128

/*-----------------------------------------------------------------------------
 * DEFINICAO DAS ESTRUTURAS DE DADOS
 *----------------------------------------------------------------------------- */
typedef struct Node Node;
typedef struct Adj Adj;

struct Node{
    int value;
    struct Node * predecessor;
    char color;
    char cicle_color;
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

typedef struct Stack{
    int max_size;
    int top;
    int end;
    Node **stack_nodes;
} Stack;


/*-----------------------------------------------------------------------------
 * PRAGMAS
 *----------------------------------------------------------------------------- */

void create_adj(Graph* graph,int i, int j);
void print_graph(Graph *graph);
int create_stack(Stack *s);
void push(Stack *s, Node * node);
int empty_stack(Stack * s);
Node *pop(Stack *s);
int dfs_visit_stack(Graph *graph, Node *node);
int dfs_stack(Graph *graph);


/*-----------------------------------------------------------------------------
 * LOGICA PRINCIPAL: LEITURA E ESCRITA DE DADOS
 *----------------------------------------------------------------------------- */

int main(void){
    int m=0,n=0;                /* numero de moleculas ligacoes */
    int i=0,j=0;                /* ligacao: molecula i se liga na j*/
    Graph graph;                /* estrutura que contera a molecula e ligacoes*/
    int test = -1;              /* 1 se a pilula eh veneno 0 caso ela deva ser testada*/
    int n_adj=0;                /* variavel auxiliar*/
    Adj *adj_list = NULL;       /* variavel auxiliar*/
    Adj *adj_list_aux = NULL;   /* variavel auxiliar*/
    
    /* Recebe a entrada do programa*/        
    scanf("%d %d", &m, &n);

    /* Inicializa o grafo*/
    graph.nodes = (Node*)malloc(sizeof(Node)*N_NODES);
    if(graph.nodes == NULL){return 1;}
    graph.n_nodes = N_NODES;
    for(i = 0; i < N_NODES; i++){
        graph.nodes[i].value = i+1;                /* rotulo do veritice*/
        graph.nodes[i].predecessor = NULL;         /* predecessor na arv da DFS*/
        graph.nodes[i].adj_list = NULL;            /* listas de adj de cada no*/
        graph.nodes[i].color = WHITE;              /* cores para dfs*/
        graph.nodes[i].cicle_color = INITIAL_COLOR;/* cores para verificar existenicia de ciclo impar*/
    }

    /* Cria adjacencias*/
    while(n_adj < N_ADJ){
        scanf("%d %d", &i, &j);
        create_adj(&graph, i-1, j-1);
        n_adj++;
    }


    /* Rotina DFS para verificar existencia de ciclos impares*/
    test = dfs_stack(&graph); 
    if(test == 0){
        printf("doturacu ou dotutama\n");
    }

    /* Desaloca memoria dinamica das variaveis*/
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
            printf("(%d, color %d, cicle_color %d) ", temp->node->value,
                   temp->node->color,temp->node->cicle_color);
            temp = temp->next;
        }
        printf("\n");
    }
}



/* Inicializa uma pilha
 * Argumentos:
 *     s: um apontador para uma struct do tipo Stack
 *  Retorno:
 *      0: em caso de sucesso
 *      1: caso tenha ocorrido erro na alocacao de memoria
 */
int create_stack(Stack *s){
    int ret = 0;
    s->max_size = STACK_MAX_SIZE;
    s->top = -1;
    s->stack_nodes = (Node**)malloc(sizeof(Node*)*STACK_MAX_SIZE);
    if(s->stack_nodes == NULL){ret = 1;}
    return ret;
}



/* Empilha um elemento do tipo Node*
 * Argumentos:
 *      s: apontador para a pilha
 *      node: apontador para um tipo Node
 * Retorno:
 *      void
 */
void push(Stack *s, Node * node){
    s->top = (s->top) + 1;
    s->stack_nodes[s->top] = node;
    if (s->top >= (((s->max_size)-sizeof(Node*))/sizeof(Node*))){
        (s->max_size) *= 2;
        s->stack_nodes = (Node**)realloc(s->stack_nodes,(sizeof(Node*)*(s->max_size)));
    }
}



/* Veririca se uma pilha esta vazia
 * Argumentos:
 *      s: ponteiro para a pilha
 * Retorno:
 *      0: caso nao esteja vazia, quaquer outro valor c.c.
 */
int empty_stack(Stack * s){
    return (s->top == -1);
}



/* Retorna o elemento do topo da pilha e o exclui dessa
 * Argumentos:
 *      s: ponteiro para a pilha
 * Retorno:
 *      elemento no topo da pilha
 */
Node *pop(Stack *s){
    int old_top = s->top;
    Node *ret = NULL;
    if(!empty_stack(s)){
        ret = (s->stack_nodes)[old_top];
        s->top = (s->top)-1;
    }
    return ret;
}



/* Modificacao da rotina DFS-VISIT apresentada no Cormen para detectar a
 * existencia de ciclos impares em um grafo nao orientado
 * Argumentos:
 *      graph: apontador para o grafo
 *      node: um apontador para o tipo Node
 * Retorno:
 *      0 caso nao haja um ciclo impar, i.e. a molecula deve ser testada
 *      1 c.c.
 */
int dfs_visit_stack(Graph *graph, Node *node){
    int i;
    int ret = 0;
    Stack s;
    Node *u, *v;
    char next_color = INITIAL_COLOR;
    Adj *edge = NULL;

    create_stack(&s);
    push(&s, node);

    while(!empty_stack(&s)){
        u = pop(&s);
        if(u->color == WHITE){
            u->color = GRAY;
            if(u->predecessor != NULL){
                u->cicle_color = ((u->predecessor->color) + 1)%2;
            }else{
                u->cicle_color = WHITE;
            }
        }
        edge = u->adj_list;
        i = 0;
        while(edge != NULL){
            v = edge->node;
            if(v != NULL && v->color == WHITE){
                v->color = GRAY;
                v->predecessor = u;
                v->cicle_color = ((u->cicle_color)+1)%2;
                push(&s,v);
            }
            else if(v != NULL && v->color == GRAY){
                next_color = ((u->cicle_color) + 1)%2;
                if(v->cicle_color != next_color){
                    printf("dotutama\n");
                    return 1;
                 }
            }
            edge = edge->next;
            i = i+1;
        }
    }
    if(s.stack_nodes != NULL){ free(s.stack_nodes);}
    return ret;
}


int dfs_stack(Graph *graph){
    int ret = 0;
    int i = 0;
    for(i = 0; i < graph->n_nodes; i++){
        if(graph->nodes[i].color == WHITE){
            ret = dfs_visit_stack(graph, &graph->nodes[i]);
            if(ret > 0){break;}
        }
    }
    return ret;
}
