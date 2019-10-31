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
#define BLACK 2
#define INITIAL_COLOR -1
#define STACK_MAX_SIZE 1

/* ESTRUTURAS DE DADOS*/
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
            printf("(%d, color %d, cicle_color %d) ", temp->node->value,temp->node->color,temp->node->cicle_color);
            temp = temp->next;
        }
        printf("\n");
    }
}

int create_stack(Stack *s){
    int ret = 0;
    s->max_size = STACK_MAX_SIZE;
    s->top = -1;
    s->stack_nodes = (Node**)malloc(sizeof(Node*)*STACK_MAX_SIZE);
    if(s->stack_nodes == NULL){ret = 1;}
    return ret;
}

void push(Stack *s, Node * node){
    s->top = (s->top) + 1;
    s->stack_nodes[s->top] = node;
    //printf("STACK TOP %d\n", s->top);
    //printf("Expression %d\n",  (((s->max_size)-sizeof(Node*))/sizeof(Node*)));
    if (s->top >= (((s->max_size)-sizeof(Node*))/sizeof(Node*))){
        (s->max_size) *= 2;
        //printf("NEW MAX SIZE: %d\n", s->max_size);
        s->stack_nodes = (Node**)realloc(s->stack_nodes,(sizeof(Node*)*(s->max_size)));
        //fflush(stdout);printf("HERE\n");
    }
    //printf("STACK TOP %d\n", s->top);
}

int empty_stack(Stack * s){
    return (s->top == -1);
}

Node *pop(Stack *s){
    int old_top = s->top;
    Node *ret = NULL;
    if(!empty_stack(s)){
        //printf("NOT NULL\n");
        ret = (s->stack_nodes)[old_top];
        s->top = (s->top)-1;
    }
    //printf("NULLLLLLL\n");
    return ret;
}



int dfs_visit_stack(Graph *graph, Node *node){
    int i;
    int ret = 0;
    Stack s;
    Node *u, *v;
    char next_color = INITIAL_COLOR;
    Adj *edge = NULL, *old_edge = NULL;

    create_stack(&s);

    /*sair*/
    push(&s, node);

    while(!empty_stack(&s)){
        //fflush(stdout);printf("Entered HERE!\n");
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
                v->predecessor = u;
                v->cicle_color = ((u->cicle_color)+1)%2;
                push(&s,v);
            }
            else if(v != NULL && v->color == GRAY){
                next_color = ((u->color) + 1)%2;
                if(v->cicle_color != next_color){
                    printf("VENENO\n");
                    return 1;
                 }
            }
            old_edge = edge;
            edge = edge->next;
            i = i+1;
        }
    }
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

int main(void){
    int m=0,n=0;
    int i=0,j=0;
    int n_adj=0;
    Graph graph;
    Node *node;
    int ret = -1;
    /* Recebe a entrada do programa*/        
    scanf("%d %d", &m, &n);
    //printf("m = %d, n = %d\n", m, n);

    /* Inicializa o grafo*/
    graph.nodes = (Node*)malloc(sizeof(Node)*N_NODES);
    if(graph.nodes == NULL){
        //printf("Error, malloc failed!\n");
        return 1;
    }
    graph.n_nodes = N_NODES;
    for(i = 0; i < N_NODES; i++){
        graph.nodes[i].value = i+1;
        graph.nodes[i].predecessor = NULL;
        graph.nodes[i].adj_list = NULL;
        graph.nodes[i].color = WHITE;
        graph.nodes[i].cicle_color = INITIAL_COLOR;
    }

    /* Cria adjacencias*/
    while(n_adj < N_ADJ){
        scanf("%d %d", &i, &j);
        //printf("i = %d, j = %d\n", i, j);
        create_adj(&graph, i-1, j-1);
        n_adj++;
    }


    /* DFS*/
    ret = dfs_stack(&graph); 
    if(ret == 0){
        printf("TESTAR\n");
    }
    /* Free nas variaveis*/
    print_graph(&graph);

    return 0;
}
