#ifndef LISTA_DE_H
#define LISTA_DE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../itens/ingredientes.h"
#include "../include/graphycs_all.h"

typedef struct {
Ingrediente ing;
Objeto* obj;
Objeto* titulo;
Objeto* preco;
}ItemDaLista;

typedef ItemDaLista tp_item_lista_DE;

void custom_print_lista(tp_item_lista_DE x)
{
	printf("%s ", x.ing.nome);
}

bool custom_comp_lista (tp_item_lista_DE a, tp_item_lista_DE b)
{
	return a.ing.id==b.ing.id;
}

typedef struct NodoLista {
	tp_item_lista_DE info;
	struct NodoLista* ant;
	struct NodoLista* prox;
} NodoLista;

typedef struct {
	NodoLista* ini;
	NodoLista* fim;
	int tam;
} Lista_DE;

Lista_DE* inicializa_lista ()
{
	Lista_DE* l = (Lista_DE*)malloc(sizeof(Lista_DE));
    if (l == NULL)
    {
        fprintf(stderr, "Erro ao alocar memória para a lista\n");
        return l;
    }
	l->ini = l->fim = NULL;
	l->tam = 0;
	return l;
}

NodoLista* nodo_alloc ()
{
	return (NodoLista*)malloc(sizeof(NodoLista));
}

bool lista_vazia (Lista_DE* l)
{
	return l->tam == 0;
}

bool insere_fim (Lista_DE* l, tp_item_lista_DE x)
{
	NodoLista* novo = nodo_alloc();
	if (novo == NULL) return false;
	novo->info = x;
	novo->prox = NULL;
	
	if (lista_vazia (l))
	{
		novo->ant = NULL;
		l->fim = l->ini = novo;
	}
	else
	{
		novo->ant = l->fim;
		l->fim->prox = novo;
		l->fim = novo;
	}
	l->tam++;
	return true;
}

bool remover (Lista_DE* l, tp_item_lista_DE x)
{
	if (lista_vazia(l)) return false;
	
	NodoLista* atual = l->ini;
	while (atual != NULL && !custom_comp_lista(atual->info,x)) atual = atual->prox;
	if (atual == NULL) return false;
	
	if (l->tam == 1)
	{
		l->ini = l->fim = NULL;
	}
	else
	{
		if (l->ini == atual)
		{
			l->ini = atual->prox;
			atual->prox->ant = NULL;
		}
		else if (l->fim == atual)
		{
			l->fim = atual->ant;
			atual->ant->prox = NULL;
		}
		else
		{
			atual->prox->ant = atual->ant;
			atual->ant->prox = atual->prox;
		}
	}
	free(atual);
	l->tam--;
    return true;
}

NodoLista* buscar_item (Lista_DE* l, tp_item_lista_DE x)
{
	if (lista_vazia(l)) return NULL;
	NodoLista* atual = l->ini;
	while (atual != NULL && !custom_comp_lista(atual->info,x)) atual = atual->prox;
	return atual;
}

Lista_DE* destruir_lista (Lista_DE* l)
{
	NodoLista* atual = l->ini;
	while (atual != NULL)
	{
		l->ini =atual->prox;
		free(atual);
		atual = l->ini;
	}
	free(l);
	return NULL;
}

void imprime_lista(Lista_DE* l)
{
    NodoLista* atual = l->ini;
    while (atual != NULL)
    {
        custom_print_lista(atual->info);
        atual = atual->prox;
    }
    printf("\n");
}

#endif