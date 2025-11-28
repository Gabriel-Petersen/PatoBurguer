#ifndef ARVORE_BB
#define ARVORE_BB

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    int ing_id;
    int qtd;
} tp_item_arvore;

bool custom_menorque (tp_item_arvore a, tp_item_arvore b)
{
	return a.ing_id < b.ing_id;
}

bool custom_comp (tp_item_arvore a, tp_item_arvore b)
{
	return a.ing_id == b.ing_id;
}

void custom_print (tp_item_arvore a)
{
	printf("ID: %d -> Qtd: %d ", a.ing_id, a.qtd);
}

typedef struct Nodo {
	struct Nodo* esq;
	struct Nodo* dir;
	
	tp_item_arvore info;
} Nodo;

typedef Nodo* Arvore;

Arvore inicializa_arvore ()
{
	return NULL;
}

bool arvore_vazia (Arvore raiz)
{
	return raiz == NULL;
}

Nodo* nodo_alloc()
{
	return (Nodo*)malloc(sizeof(Nodo));
}

bool inserir (Arvore* raiz, tp_item_arvore x)
{
	Nodo* novo = nodo_alloc();
	if (novo == NULL) return false;
	
	novo->esq = novo->dir = NULL;
	novo->info = x;
	
	Nodo* pai = NULL;
	Nodo* atu = *raiz;
	while (atu != NULL)
	{
		pai = atu;
		if (custom_menorque(x, atu->info))
			atu = atu->esq;
		else
			atu = atu->dir;
	}
	
	if (pai == NULL)
	{
		*raiz = novo;
	}
	else
	{
		if (custom_menorque(x, pai->info))
			pai->esq = novo;
		else
			pai->dir = novo;
	}
	
	return true;
}

Nodo* busca (Arvore raiz, tp_item_arvore x)
{
	Nodo* atu = raiz;
	while (atu != NULL)
	{
		if (custom_comp(x, atu->info)) return atu;
		if (custom_menorque(x, atu->info))
			atu = atu->esq;
		else
			atu = atu->dir;
	}
	return atu;
}

void print_infixo (Arvore raiz)
{
	if (raiz == NULL) return;
	print_infixo(raiz->esq);
	custom_print(raiz->info); putchar('\n');
	print_infixo(raiz->dir);
}

int qtd_niveis(Arvore raiz)
{
	if (raiz == NULL) return 0;
	int esq = qtd_niveis(raiz->esq);
	int dir = qtd_niveis(raiz->dir);
	
	return 1 + (esq > dir ? esq : dir);
}

int qtd_nos(Arvore raiz)
{
	return raiz == NULL ? 0 : (1 + qtd_nos(raiz->dir) + qtd_nos(raiz->esq));
}

int remover_no (Arvore* raiz, tp_item_arvore x)
{
	Nodo* atu = *raiz;
	Nodo* ant = NULL;
	while (atu != NULL && custom_comp(atu->info, x) == false)
	{
		ant = atu;
		if (custom_menorque(x, atu->info))
			atu = atu->esq;
		else
			atu = atu->dir;
	}
	
	if (atu == NULL) return 0; // x não encontrado
	
	Nodo* sub;
	Nodo* filho;
	if (atu->esq == NULL)
	{
		sub = atu->dir;
	}
	else if (atu->dir == NULL)
	{
		sub = atu->esq; 
	}
	else
	{
		Nodo* pai = atu;
		sub = atu->dir;
		filho = sub->esq;
		while (filho != NULL)
		{
			pai = sub;
			sub = filho;
			filho = sub->esq;
		}
		
		if (pai != atu)
		{
			pai->esq = sub->dir;
			sub->dir = atu->dir;
		}
		sub->esq = atu->esq;
	}

    if (ant == NULL)
    {
        *raiz = sub;
    }
    else
    {
        if (atu == ant->esq)
            ant->esq = sub;
        else
            ant->dir = sub;
    }
    free(atu);
	
	return 1;
}

void destruir_nodo (Nodo* n)
{
    if (n == NULL) return;
    destruir_nodo(n->esq);
    destruir_nodo(n->dir);
    free(n);
}

void destruir_arvore (Arvore raiz)
{
    destruir_nodo(raiz);
}

#endif