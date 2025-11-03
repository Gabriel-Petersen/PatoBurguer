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

void custom_print(tp_item_lista_DE x)
{
	printf("%s ", x.ing.nome);
}

bool custom_comp (tp_item_lista_DE a, tp_item_lista_DE b)
{
	return a.ing.id==b.ing.id;
}

typedef struct Nodo {
	tp_item_lista_DE info;
	struct Nodo* ant;
	struct Nodo* prox;
} Nodo;

typedef struct {
	Nodo* ini;
	Nodo* fim;
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

Nodo* node_alloc ()
{
	return (Nodo*)malloc(sizeof(Nodo));
}

bool lista_vazia (Lista_DE* l)
{
	return l->tam == 0;
}

bool insere_fim (Lista_DE* l, tp_item_lista_DE x)
{
	Nodo* novo = node_alloc();
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
	
	Nodo* atual = l->ini;
	while (atual != NULL && !custom_comp(atual->info,x)) atual = atual->prox;
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

Nodo* buscar_item (Lista_DE* l, tp_item_lista_DE x)
{
	if (lista_vazia(l)) return NULL;
	Nodo* atual = l->ini;
	while (atual != NULL && !custom_comp(atual->info,x)) atual = atual->prox;
	return atual;
}

Lista_DE* destruir_lista (Lista_DE* l)
{
	Nodo* atual = l->ini;
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
    Nodo* atual = l->ini;
    while (atual != NULL)
    {
        custom_print(atual->info);
        atual = atual->prox;
    }
    printf("\n");
}

#endif