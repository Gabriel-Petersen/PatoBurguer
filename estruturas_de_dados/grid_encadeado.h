#ifndef GRID_ENCADEADO
#define GRID_ENCADEADO
#define USE_SHORTCUTS
#include "../include/graphycs_all.h"

typedef struct NodoGrid {
    int ing_id;
    Vector2 pos_ini;
    Vector2 tam;

    struct NodoGrid* cima;
    struct NodoGrid* baixo;
    struct NodoGrid* esq;
    struct NodoGrid* dir;
} NodoGrid;

typedef struct {
    NodoGrid* ini;
    NodoGrid* atu;
} GridEncadeado;

NodoGrid* criar_nodo_grid (int id, Vector2 pos_ini, Vector2 tam)
{
    NodoGrid* novo = (NodoGrid*)malloc(sizeof(NodoGrid));
    if (novo == NULL)
    {
        printf("Erro ao alocar memoria para Nodo do Grid\n");
        return novo;
    }

    novo->cima = novo->baixo = novo->dir = novo->esq = NULL;
    novo->ing_id = id;
    novo->pos_ini = pos_ini;
    novo->tam = tam;
    return novo;
}

GridEncadeado* inicializa_grid_encadeado ()
{
    GridEncadeado* g = (GridEncadeado*)malloc(sizeof(GridEncadeado));
    if (g != NULL)
        g->ini = g->atu = NULL;
    else
        printf("Erro ao alocar memoria para GridEncadeado\n");
    return g;
}

GridEncadeado* criar_grid_encadeado()
{
    GridEncadeado* g = inicializa_grid_encadeado();
    if (g == NULL) return NULL;

    const Vector2 ret_size = nv2(15, 5);
    const Vector2 ini1 = nv2(1, 1);
    const Vector2 ini2 = nv2(115, 1);
    const int passo = ret_size.y + 1;
    
    const Vector2 gaveta_extra_pos = nv2(46, 0); 
    const Vector2 tam_gaveta = nv2(40, 4); 

    const Vector2 molho_area = nv2(7, 11); 

    // --- Criação dos Nodos ---
    // Coluna Esquerda:
    NodoGrid* n1_topo = criar_nodo_grid(0, ini1, ret_size); // Topo Pão
    NodoGrid* n2_carne = criar_nodo_grid(2, nv2(ini1.x, ini1.y + 1*passo), ret_size);
    NodoGrid* n3_bacon = criar_nodo_grid(3, nv2(ini1.x, ini1.y + 2*passo), ret_size);
    NodoGrid* n4_queijo = criar_nodo_grid(4, nv2(ini1.x, ini1.y + 3*passo), ret_size);
    NodoGrid* n5_ovo = criar_nodo_grid(11, nv2(ini1.x, ini1.y + 4*passo), ret_size); // Ovo Frito

    // Coluna Direita:
    NodoGrid* n7_alface = criar_nodo_grid(5, ini2, ret_size); // Alface
    NodoGrid* n8_tomate = criar_nodo_grid(6, nv2(ini2.x, ini2.y + 1*passo), ret_size);
    NodoGrid* n9_cebola = criar_nodo_grid(9, nv2(ini2.x, ini2.y + 2*passo), ret_size);
    NodoGrid* n10_picles = criar_nodo_grid(10, nv2(ini2.x, ini2.y + 3*passo), ret_size);
    NodoGrid* n11_onion = criar_nodo_grid(12, nv2(ini2.x, ini2.y + 4*passo), ret_size); // Onion Rings

    // Base de Pão e Molhos
    NodoGrid* n0_base = criar_nodo_grid(1, gaveta_extra_pos, tam_gaveta); // Base Pão (Centro Superior)
    NodoGrid* n6_maionese = criar_nodo_grid(7, nv2(27, 14), molho_area); // Molho 1 (Esquerda)
    NodoGrid* n12_barbecue = criar_nodo_grid(8, nv2(97, 14), molho_area); // Molho 2 (Direita)

    // --- Conexões Verticais (Triviais) ---
    n1_topo->baixo = n2_carne; n2_carne->cima = n1_topo;
    n2_carne->baixo = n3_bacon; n3_bacon->cima = n2_carne;
    n3_bacon->baixo = n4_queijo; n4_queijo->cima = n3_bacon;
    n4_queijo->baixo = n5_ovo; n5_ovo->cima = n4_queijo;

    n7_alface->baixo = n8_tomate; n8_tomate->cima = n7_alface;
    n8_tomate->baixo = n9_cebola; n9_cebola->cima = n8_tomate;
    n9_cebola->baixo = n10_picles; n10_picles->cima = n9_cebola;
    n10_picles->baixo = n11_onion; n11_onion->cima = n10_picles;

    // --- Conexões Especiais (Baseadas na sua lógica) ---
    
    // **A. BASE DE PÃO (n0_base)**
    n0_base->baixo = n6_maionese; 
    n0_base->dir = n7_alface; 
    n0_base->esq = n1_topo; 

    // **B. MOLHOS (n6_maionese / n12_barbecue)**
    n6_maionese->cima = n0_base;
    n12_barbecue->cima = n0_base;
    n6_maionese->dir = n12_barbecue;
    n12_barbecue->esq = n6_maionese;

    n6_maionese->esq = n3_bacon;
    n12_barbecue->dir = n9_cebola;

    // **C. VERTICAIS ESQUERDA (n1_topo a n5_ovo)**

    n1_topo->dir = n0_base; // topo_de_pão -> base_de_pão
    n2_carne->dir = n6_maionese; // carne -> maionese
    n3_bacon->dir = n6_maionese; // bacon -> maionese
    n4_queijo->dir = n6_maionese; // queijo -> maionese
    n5_ovo->dir = n6_maionese; // ovo -> maionese
    
    // **D. VERTICAIS DIREITA (n7_alface a n11_onion)**

    n7_alface->esq = n0_base; // alface -> base_de_pão
    n8_tomate->esq = n12_barbecue; // tomate -> barbecue
    n9_cebola->esq = n12_barbecue; // cebola -> barbecue
    n10_picles->esq = n12_barbecue; // picles -> barbecue
    n11_onion->esq = n12_barbecue; // onion -> barbecue

    // **E. Conexões de Retorno (Molhos/Base -> Verticais)**
    n1_topo->cima = n0_base; // Corrige o nulo que seria do topo
    n7_alface->cima = n0_base; // Corrige o nulo que seria do topo

    // --- Configuração Final do Grid ---
    g->ini = n1_topo; 
    g->atu = n1_topo; // Começamos no Topo de Pão

    return g;
}

void percorrer_grid_dfs (NodoGrid* nodos[], NodoGrid* n)
{
    if (n == NULL || nodos[n->ing_id] != NULL) return;
    
    nodos[n->ing_id] = n;
    percorrer_grid_dfs(nodos, n->cima);
    percorrer_grid_dfs(nodos, n->baixo);
    percorrer_grid_dfs(nodos, n->dir);
    percorrer_grid_dfs(nodos, n->esq);
}

void destruir_grid_encadeado (GridEncadeado* g)
{
    g->atu = g->ini;
    NodoGrid* nodos[13] = {NULL};
    percorrer_grid_dfs(nodos, g->ini);
    for (int i = 0; i < 13; i++) free(nodos[i]);
    free(g);
}

#endif