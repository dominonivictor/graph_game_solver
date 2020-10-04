// NOMES DOS RESPONSÁVEIS: Victor Dominoni
//--------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

// ######### ESCREVA O NROUSP DO PRIMEIRO INTEGRANTE AQUI
int nroUSP1() {
    return 8063371;
}

// ######### ESCREVA O NROUSP DO SEGUNDO INTEGRANTE AQUI (OU DEIXE COM ZERO)
int  nroUSP2() {
    return 0;
}

// ######### ESCREVA O NRO DO GRUPO CADASTRADO
int grupo() {
    return 0;
}

typedef struct {
	int bot_lin;
	int bot_col;
	int exit_lin;
	int exit_col;
	int box_col;
	int key_col;
	bool haskey;
}ESTADO;

typedef struct s{
    int v_pos;
    char action;
    struct s* prox;
}EDGE;

typedef struct {
    ESTADO state;
    int pos;
    int status;
    EDGE* first;
    int via;
}VERTEX;

// funcao principal
void caminhoValido(ESTADO* s, char* resp);

// funcoes auxiliares
//
void build_resp(VERTEX* l, char* resp, int i, int via, int start, int end, int* counter);
void show_resp(char*, int);
void depth(VERTEX*, int, int, bool*, char*, int*, int);
int is_same_node(ESTADO, ESTADO);
ESTADO action_function(char, ESTADO, int*);
VERTEX* create_graph_from_initial_values(ESTADO, int*, int*, int*);
ESTADO new_state(int, int, int, int, int, int, bool);
int exit(ESTADO*);
int grab(ESTADO*);
int push_right(ESTADO*);
int push_left(ESTADO*);
int go_down(ESTADO*);
int go_up(ESTADO*);
int walk_right(ESTADO*);
int walk_left(ESTADO*);
//------------------------------------------
// O EP consiste em implementar esta funcao
// e outras funcoes auxiliares que esta
// necessitar
//------------------------------------------
void show_resp(char* l, int size){
    for(int i = 0; i < size; i++){
        printf("i: %d, action: %c\n", i, l[i]);
    }
}
void caminhoValido(ESTADO* s, char* resp){
    int total_vertices;
    int start_i;
    int end_i;
    VERTEX* list = create_graph_from_initial_values(*s, &total_vertices, &start_i, &end_i);
    bool found= false;
    int counter = 0;
    depth(list, start_i, end_i, &found, resp, &counter, -1);
    int r_counter;
    build_resp(list, resp, end_i, -1, start_i, end_i, &r_counter); 
    show_resp(resp, r_counter);
}

void build_resp(VERTEX* l, char* resp, int i, int via, int start, int end, int* counter){
    if(i == start){
        *counter = 0;
    }else{
        build_resp(l, resp, l[i].via, i, start, end, counter);
    }
    if(i == end){
        resp[*counter] = 'e';
        *counter += 1;
        return;
    }
    EDGE* p = l[i].first;
    while(p->v_pos != via){
        p=p->prox;
    }
    resp[*counter] = p->action; 
    *counter += 1;
}

void depth(VERTEX* g, int i, int end, bool* found, char* resp, int* counter, int via){
    g[i].via = via;
    if(i == end){
        *found = true;
        g[i].status = 2;
        return;
    }
    if(*found) return;
    g[i].status = 1;
    EDGE* p = g[i].first;
    while(p){
        if(g[p->v_pos].status == 0){
            resp[*counter] = p->action;
            *counter *= 1;
            depth(g, p->v_pos, end, found, resp, counter, i);
        }
        p = p->prox;
    }
    g[i].status = 2;
}

int walk_left(ESTADO* s){
    int valid = 0;
    if(s->bot_col > 1){
       s->bot_col -= 1; 
       if(s->bot_lin==2){
           s->bot_lin += 1;
        }
        valid = 1;
    }
    return valid;
}

int walk_right(ESTADO* s){
    int valid = 0;
    if(s->bot_col < 3){
       s->bot_col += 1; 
       if(s->bot_lin==2){
           s->bot_lin += 1;
        }
       valid = 1;
    }
    return valid;
}

int go_up(ESTADO* s){
    int valid = 0;
    if(s->bot_col == s->box_col && s->bot_lin==3){
        s->bot_lin = 2;
       valid = 1;
    }
    return valid;
}

int go_down(ESTADO* s){
    int valid = 0;
    if(s->bot_lin==2 && s->bot_col == s->box_col){
        s->bot_lin = 3;
        valid = 1;
    }
    return valid;
}

int push_left(ESTADO* s){
    int valid = 0;
    if(s->bot_lin==3 && s->bot_col==s->box_col && s->bot_col > 1){
        s->bot_col -= 1;
        s->box_col -= 1;
       valid = 1;
    }
    return valid;
}

int push_right(ESTADO* s){
    int valid = 0;
    if(s->bot_lin==3 && s->bot_col==s->box_col && s->bot_col < 3){
        s->bot_col += 1;
        s->box_col += 1;
       valid = 1;
    }
    return valid;
}

int grab(ESTADO* s){
    int valid = 0;
    if(s->bot_lin==2 && s->bot_col == s->key_col){
        s->haskey = true;
       s->key_col = 0;
       valid = 1;
    }
    return valid;
}

int exit(ESTADO* s){
    int valid = 0;
    if(s->bot_lin == s->exit_lin && s->haskey && s->bot_col==s->exit_col){
       valid = 1;
    }
    return valid;
}

ESTADO new_state(int b_l, int b_c, int e_l, int e_c, int x_c, int k_c, bool haskey){
    ESTADO state;
    state.bot_lin = b_l;
    state.bot_col = b_c;
    state.exit_lin = e_l;
    state.exit_col = e_c;
    state.box_col = x_c;
    state.key_col = k_c;
    state.haskey = haskey;

    return state;
}

VERTEX* create_graph_from_initial_values(ESTADO estado_inicial, int* vertex_count, int* start_pos, int* end_pos){
    int MAX_VERTICES = 50;
    VERTEX* v_list = (VERTEX*) (malloc(sizeof(VERTEX)*MAX_VERTICES));
    int key_col = estado_inicial.key_col;
    int exit_col = estado_inicial.exit_col;
    int exit_lin = estado_inicial.exit_lin;
    
    char action_list[] = {'d', 'l', 'r', 'L', 'R', 'u', 'g', 'e'};
    int action_list_len = 8;
    int count = 0;
    //building all possible vertices
    for(int i = 1; i <= 3; i++){//bot x positions
        for(int j = 2; j <= 3; j++){//bot y positions
            for(int k = 1; k <= 3; k++){//box x positions
                if((j==2 && i == k) || j==3){//bot can only be on 2nd line if box is directly beneath
                    ESTADO state_without_key = new_state(j, i, exit_lin, exit_col, k, 0, true);
                    VERTEX v;
                    v.state = state_without_key;
                    v.pos = count;
                    v.status = 0;
                    v.first = NULL;
                    v_list[count] = v; 
                    count += 1;
                    ESTADO state_with_key = new_state(j, i, exit_lin, exit_col, k, key_col, false);
                    VERTEX v2;
                    v2.state = state_with_key;
                    v2.pos = count;
                    v2.status = 0;
                    v2.first = NULL;
                    v_list[count] = v2;
                    count += 1;
                }
            }
        }
    }
    //building all vertices neighbors
    *vertex_count = count; 
    for(int i = 0; i < count; i++){
        int first = 0;
        EDGE* last_edge = (EDGE*) malloc(sizeof(EDGE)); 
        for(int j = 0; j < action_list_len; j++){
            int valid= 0;
            ESTADO neighbor_state = action_function(action_list[j], v_list[i].state, &valid);
            if(valid == 1){
               EDGE* new_edge = (EDGE*) malloc(sizeof(EDGE)); 
               for(int v_pos = 0; v_pos < *vertex_count; v_pos++){
                   if(is_same_node(neighbor_state, v_list[v_pos].state)){
                       if(is_same_node(neighbor_state, estado_inicial)){
                           *start_pos = v_pos;
                        }
                        if(action_list[j] == 'e'){
                            *end_pos = v_pos;
                        }
                       new_edge->v_pos = v_pos;
                       new_edge->action = action_list[j];

                       if(first != 0){
                           last_edge->prox = new_edge;
                       }else{
                           v_list[i].first = new_edge;
                           first = 1;
                       }
                       last_edge = new_edge;
                    }
                }
            }
        }
        last_edge->prox = NULL;
    }

    return v_list;
    
}

ESTADO action_function(char action_char, ESTADO state, int* valid){
    ESTADO new_vertex = state;
    switch(action_char){
        case 'u':
            *valid = go_up(&new_vertex); 
            break;
        case 'l':
            *valid = walk_left(&new_vertex); 
            break;
        case 'r':
            *valid = walk_right(&new_vertex); 
            break;
        case 'L':
            *valid = push_left(&new_vertex); 
            break;
        case 'R':
            *valid = push_right(&new_vertex); 
            break;
        case 'd':
            *valid = go_down(&new_vertex); 
            break;
        case 'g':
            *valid = grab(&new_vertex); 
            break;
        case 'e':
            *valid = exit(&new_vertex); 

            break;
    }
    return new_vertex;

}

int is_same_node(ESTADO s1, ESTADO s2){
    if(s1.haskey == s2.haskey && s1.bot_col == s2.bot_col &&
        s1.bot_lin == s2.bot_lin && s1.key_col == s2.key_col &&
        s1.exit_lin == s2.exit_lin && s1.exit_col == s2.exit_col &&
        s1.box_col == s2.box_col){
        return 1;
    }
    return 0;
}
//---------------------------------------------------------
// use main() para fazer chamadas de teste ao seu programa
// mas nao entregue o codido de main() nem inclua nada
// abaixo deste ponto
//---------------------------------------------------------
