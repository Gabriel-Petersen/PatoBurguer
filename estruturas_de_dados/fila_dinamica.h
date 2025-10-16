#ifndef FILAD_H
#define FILAD_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

typedef int tp_item_fila;
typedef struct Nodo {
    tp_item_fila info;
    struct Nodo* prox;
} Nodo;

typedef struct {
    Nodo* ini;
    Nodo* fim;
    int tam;
} Fila_D;

Fila_D* inicializa_fila ()
{
    Fila_D* f = (Fila_D*)malloc(sizeof(Fila_D));
    if (f == NULL)
    {
        fprintf(stderr, "Erro ao alocar memória para a fila\n");
        return f;
    }
    f->fim = f->ini = NULL;
    f->tam = 0;
    
    return f;
}

bool fila_vazia (Fila_D* f)
{
    return f->tam == 0;
}

bool enqueue (Fila_D* f, tp_item_fila x)
{
    Nodo* novo = (Nodo*)malloc(sizeof(Nodo));
    if (novo == NULL)
    {
        fprintf(stderr, "Erro ao alocar memória para a fila\n");
        return false;
    }

    novo->info = x;
    novo->prox = NULL;
    if (!fila_vazia(f)) f->fim->prox = novo;
    f->fim = novo;
    if (fila_vazia(f)) f->ini = novo;
    f->tam++;

    return true;
}

bool dequeue (Fila_D* f, tp_item_fila* x)
{
    if (fila_vazia(f))
    {
        return false;
    }
    *x = f->ini->info;
    Nodo* antigo = f->ini;
    f->ini = f->ini->prox;
    free(antigo);
    f->tam--;

    return true;
}

bool front (Fila_D* f, tp_item_fila* x)
{
    if (fila_vazia(f))
    {
        return false;
    }
    *x = f->ini->info;
    return true;
}

void destruir_fila(Fila_D* f) 
{
    if (f == NULL) return;
    Nodo* atual = f->ini;
    while (atual != NULL) 
    {
        Nodo* temp = atual;
        atual = atual->prox;
        free(temp);
    }
    free(f);
}


#endif